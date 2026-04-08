#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { STATE_TEXT, STATE_TAG, STATE_SCRIPT, STATE_STYLE } ParserState;

typedef struct {
    char id[64];
    char type[32];
    char content[256];
    char onclick[64];
    float r, g, b, a;
    int has_color;
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

void hex_to_rgb(const char* hex, float* r, float* g, float* b) {
    if (hex[0] == '#') hex++;
    unsigned int rgb;
    sscanf(hex, "%x", &rgb);
    *r = ((rgb >> 16) & 0xFF) / 255.0f;
    *g = ((rgb >> 8) & 0xFF) / 255.0f;
    *b = (rgb & 0xFF) / 255.0f;
}

void parse_css(const char* css) {
    char* copy = strdup(css);
    char* selector = strtok(copy, "{");
    while (selector) {
        char* properties = strtok(NULL, "}");
        if (!properties) break;
        trim(selector);
        if (selector[0] == '#') {
            char id[64];
            strcpy(id, selector + 1);
            for (int i=0; i<node_count; i++) {
                if (strcmp(nodes[i].id, id) == 0) {
                    char* color_ptr = strstr(properties, "background-color:");
                    if (color_ptr) {
                        char hex[16];
                        sscanf(color_ptr, "background-color: %[^;]", hex);
                        trim(hex);
                        hex_to_rgb(hex, &nodes[i].r, &nodes[i].g, &nodes[i].b);
                        nodes[i].a = 1.0f;
                        nodes[i].has_color = 1;
                    }
                }
            }
        }
        selector = strtok(NULL, "{");
    }
    free(copy);
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
                else if (strncmp(p, "<style>", 7) == 0) { state = STATE_STYLE; p += 6; }
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
                if (click_ptr) { sscanf(click_ptr, "onclick=\"%[^(]", n.onclick); trim(n.onclick); }
                nodes[node_count++] = n; p += 5;
            } else if (strncmp(p, "p", 1) == 0 && (isspace(p[1]) || p[1] == '>')) {
                Node n; memset(&n, 0, sizeof(Node));
                strcpy(n.type, "ELEMENT_TYPE_TEXT");
                char* id_ptr = strstr(p, "id=\""); if (id_ptr) sscanf(id_ptr, "id=\"%[^\"]\"", n.id);
                nodes[node_count++] = n;
            }
        } else if (state == STATE_SCRIPT) {
            if (strncmp(p, "</script>", 9) == 0) { state = STATE_TEXT; p += 8; }
        } else if (state == STATE_STYLE) {
            if (strncmp(p, "</style>", 8) == 0) { state = STATE_TEXT; p += 7; }
        }
        p++;
    }
}

void transpile_js_line(FILE* out, char* line) {
    trim(line); if (strlen(line) == 0) return;
    if (strncmp(line, "let ", 4) == 0) {
        char name[64], val[64];
        if (sscanf(line, "let %s = %[^;]", name, val) == 2) fprintf(out, "    float %s = %s;\n", name, val);
    }
    else if (strstr(line, ".style.backgroundColor =")) {
        char id[64], hex[16];
        char* id_s = strchr(line, '\'') + 1;
        char* id_e = strchr(id_s, '\'');
        strncpy(id, id_s, id_e - id_s); id[id_e - id_s] = '\0';
        char* val_s = strrchr(line, '\'') - 7;
        if (val_s[0] == '#') {
            float r, g, b; hex_to_rgb(val_s, &r, &g, &b);
            fprintf(out, "    { Element* el = element_find_by_id(win->root, \"%s\"); if(el) element_set_bg_color(el, %.2ff, %.2ff, %.2ff, 1.0f); }\n", id, r, g, b);
        }
    }
    else if (strstr(line, ".innerText =")) {
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
    else if (strstr(line, " = ") && strchr(line, '+')) { // Asignacion con suma simple
         fprintf(out, "    %s;\n", line);
    }
}

void transpile(const char* i_f, const char* o_f) {
    FILE* in = fopen(i_f, "r"); FILE* out = fopen(o_f, "w"); if (!in || !out) return;
    fseek(in, 0, SEEK_END); long len = ftell(in); fseek(in, 0, SEEK_SET);
    char* buf = (char*)malloc(len + 1); fread(buf, 1, len, in); buf[len] = '\0'; fclose(in);
    fprintf(out, "#include \"engine.h\"\n#include \"gc.h\"\n#include <stdio.h>\n\nWindow* win;\n");
    char* js_start = strstr(buf, "<script>");
    char* js_end = strstr(buf, "</script>");
    if (js_start && js_end) {
        char* j_c = strndup(js_start + 8, js_end - js_start - 8); char* l_s = j_c, *l_e;
        while ((l_e = strchr(l_s, '\n'))) { *l_e = '\0'; transpile_js_line(out, l_s); l_s = l_e + 1; }
        transpile_js_line(out, l_s); free(j_c);
    }
    parse_app(buf);
    char* css_start = strstr(buf, "<style>"); char* css_end = strstr(buf, "</style>");
    if (css_start && css_end) { char* css_c = strndup(css_start + 7, css_end - css_start - 7); parse_css(css_c); free(css_c); }
    fprintf(out, "\nint main() {\n    engine_init();\n    win = window_create(800, 600, \"TPTC Native App\");\n");
    for (int i = 0; i < node_count; i++) {
        fprintf(out, "    Element* el%d = element_create(%s, \"%s\");\n", i, nodes[i].type, nodes[i].id);
        if (strlen(nodes[i].content) > 0) {
            char sanitized[256] = {0}; int sj = 0;
            for(int si=0; nodes[i].content[si] && sj < 250; si++) {
                if(nodes[i].content[si] == '\n') { sanitized[sj++] = '\\'; sanitized[sj++] = 'n'; }
                else if(nodes[i].content[si] == '"') { sanitized[sj++] = '\\'; sanitized[sj++] = '"'; }
                else sanitized[sj++] = nodes[i].content[si];
            }
            fprintf(out, "    element_set_text(el%d, \"%s\");\n", i, sanitized);
        }
        if (nodes[i].has_color) fprintf(out, "    element_set_bg_color(el%d, %.2ff, %.2ff, %.2ff, %.2ff);\n", i, nodes[i].r, nodes[i].g, nodes[i].b, nodes[i].a);
        fprintf(out, "    element_add_child(win->root, el%d);\n", i);
        if (strlen(nodes[i].onclick) > 0) fprintf(out, "    el%d->on_click = (void (*)(struct Element*))%s;\n", i, nodes[i].onclick);
    }
    fprintf(out, "    engine_run(win);\n    engine_terminate();\n    return 0;\n}\n");
    fclose(out); free(buf);
}
int main(int argc, char** argv) { if (argc < 3) return 1; transpile(argv[1], argv[2]); return 0; }
