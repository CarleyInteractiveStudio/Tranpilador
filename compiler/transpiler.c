#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { STATE_TEXT, STATE_TAG, STATE_SCRIPT } ParserState;

typedef struct {
    char id[64];
    char type[32];
    char content[256];
    char onclick[64];
} Node;

Node nodes[100];
int node_count = 0;

void trim(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

void parse_app(const char* input) {
    ParserState state = STATE_TEXT;
    const char* p = input;
    char current_content[256] = {0};
    int content_idx = 0;
    node_count = 0;

    while (*p) {
        if (state == STATE_TEXT) {
            if (*p == '<') {
                if (strncmp(p, "<script>", 8) == 0) { state = STATE_SCRIPT; p += 7; }
                else {
                    state = STATE_TAG;
                    current_content[content_idx] = '\0';
                    if (node_count > 0 && content_idx > 0) {
                        trim(current_content);
                        if (strlen(current_content) > 0) strcpy(nodes[node_count-1].content, current_content);
                    }
                    content_idx = 0;
                }
            } else { current_content[content_idx++] = *p; }
        } else if (state == STATE_TAG) {
            if (*p == '>') { state = STATE_TEXT; content_idx = 0; }
            else if (strncmp(p, "button", 6) == 0) {
                Node n; memset(&n, 0, sizeof(Node));
                strcpy(n.type, "ELEMENT_TYPE_BUTTON");
                char* id_ptr = strstr(p, "id=\""); if (id_ptr) sscanf(id_ptr, "id=\"%[^\"]\"", n.id);
                char* click_ptr = strstr(p, "onclick=\"");
                if (click_ptr) {
                    sscanf(click_ptr, "onclick=\"%[^(]", n.onclick);
                    trim(n.onclick);
                }
                nodes[node_count++] = n; p += 5;
            } else if (strncmp(p, "p", 1) == 0 && (isspace(p[1]) || p[1] == '>')) {
                Node n; memset(&n, 0, sizeof(Node));
                strcpy(n.type, "ELEMENT_TYPE_TEXT");
                char* id_ptr = strstr(p, "id=\""); if (id_ptr) sscanf(id_ptr, "id=\"%[^\"]\"", n.id);
                nodes[node_count++] = n;
            }
        } else if (state == STATE_SCRIPT) {
            if (strncmp(p, "</script>", 9) == 0) { state = STATE_TEXT; p += 8; }
        }
        p++;
    }
}

void transpile_js_line(FILE* out, char* line) {
    trim(line); if (strlen(line) == 0) return;
    if (strstr(line, ".innerText =")) {
        char id[64], val[256], *id_s = strchr(line, '\'');
        if (id_s) {
            id_s++; char* id_e = strchr(id_s, '\'');
            if (id_e) {
                strncpy(id, id_s, id_e - id_s); id[id_e - id_s] = '\0';
                char* v_s = strchr(id_e + 1, '\'');
                if (v_s) { v_s++; char* v_e = strchr(v_s, '\'');
                    if (v_e) { strncpy(val, v_s, v_e - v_s); val[v_e - v_s] = '\0';
                        fprintf(out, "    { Element* el = element_find_by_id(win->root, \"%s\"); if(el) element_set_text(el, \"%s\"); }\n", id, val);
                    }
                }
            }
        }
    } else if (strncmp(line, "function", 8) == 0) {
        char name[64]; sscanf(line, "function %s", name); char* p = strchr(name, '('); if(p) *p = '\0';
        fprintf(out, "void %s(Element* self) {\n", name);
    } else if (strcmp(line, "}") == 0) fprintf(out, "}\n");
}

void transpile(const char* i_f, const char* o_f) {
    FILE* in = fopen(i_f, "r"); FILE* out = fopen(o_f, "w"); if (!in || !out) return;
    fseek(in, 0, SEEK_END); long len = ftell(in); fseek(in, 0, SEEK_SET);
    char* buf = (char*)malloc(len + 1); fread(buf, 1, len, in); buf[len] = '\0'; fclose(in);
    fprintf(out, "#include \"engine.h\"\n#include \"gc.h\"\n#include <stdio.h>\n\n");
    char* js_start = strstr(buf, "<script>");
    char* js_end = strstr(buf, "</script>");
    fprintf(out, "\nWindow* win;\n");
    if (js_start && js_end) {
        char* j_c = strndup(js_start + 8, js_end - js_start - 8); char* l_s = j_c, *l_e;
        while ((l_e = strchr(l_s, '\n'))) { *l_e = '\0'; transpile_js_line(out, l_s); l_s = l_e + 1; }
        transpile_js_line(out, l_s); free(j_c);
    }
    fprintf(out, "\nint main() {\n    engine_init();\n    win = window_create(800, 600, \"TPTC Native App\");\n");
    parse_app(buf);
    for (int i = 0; i < node_count; i++) {
        fprintf(out, "    Element* el%d = element_create(%s, \"%s\");\n", i, nodes[i].type, nodes[i].id);
        if (strlen(nodes[i].content) > 0) {
            char sanitized[256] = {0};
            int sj = 0;
            for(int si=0; nodes[i].content[si] && sj < 250; si++) {
                if(nodes[i].content[si] == '\n') { sanitized[sj++] = '\\'; sanitized[sj++] = 'n'; }
                else if(nodes[i].content[si] == '"') { sanitized[sj++] = '\\'; sanitized[sj++] = '"'; }
                else sanitized[sj++] = nodes[i].content[si];
            }
            fprintf(out, "    element_set_text(el%d, \"%s\");\n", i, sanitized);
        }
        fprintf(out, "    element_add_child(win->root, el%d);\n", i);
        if (strlen(nodes[i].onclick) > 0) {
            fprintf(out, "    el%d->on_click = (void (*)(struct Element*))%s;\n", i, nodes[i].onclick);
        }
    }
    fprintf(out, "    engine_run(win);\n    engine_terminate();\n    return 0;\n}\n");
    fclose(out); free(buf);
}
int main(int argc, char** argv) { if (argc < 3) return 1; transpile(argv[1], argv[2]); return 0; }
