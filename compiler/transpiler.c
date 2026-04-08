#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { STATE_TEXT, STATE_TAG, STATE_SCRIPT } ParserState;
typedef struct { char id[64], type[32], content[256], onclick[64]; } Node;
Node nodes[100]; int node_count = 0;

void trim(char* str) {
    char* end; while(isspace((unsigned char)*str)) str++; if(*str == 0) return;
    end = str + strlen(str) - 1; while(end > str && isspace((unsigned char)*end)) end--; end[1] = '\0';
}

void parse_app(const char* input) {
    ParserState state = STATE_TEXT; const char* p = input;
    char current_content[256] = {0}; int content_idx = 0; node_count = 0;
    while (*p) {
        if (state == STATE_TEXT) {
            if (*p == '<') {
                if (strncmp(p, "<script>", 8) == 0) { state = STATE_SCRIPT; p += 7; }
                else {
                    state = STATE_TAG; current_content[content_idx] = '\0';
                    if (node_count > 0 && content_idx > 0) { trim(current_content); if (strlen(current_content) > 0) strcpy(nodes[node_count-1].content, current_content); }
                    content_idx = 0;
                }
            } else { current_content[content_idx++] = *p; }
        } else if (state == STATE_TAG) {
            if (*p == '>') { state = STATE_TEXT; content_idx = 0; }
            else {
                char tag[256]; sscanf(p, "%s", tag); Node n; memset(&n, 0, sizeof(Node)); int rec = 0;
                if (strncmp(tag, "button", 6) == 0) { strcpy(n.type, "ELEMENT_TYPE_BUTTON"); rec = 1; p += 5; }
                else if (strncmp(tag, "p", 1) == 0) { strcpy(n.type, "ELEMENT_TYPE_TEXT"); rec = 1; }
                if (rec) {
                    char* id_ptr = strstr(p, "id=\""); if (id_ptr) sscanf(id_ptr, "id=\"%[^\"]\"", n.id);
                    char* click_ptr = strstr(p, "onclick=\""); if (click_ptr) { sscanf(click_ptr, "onclick=\"%[^(]", n.onclick); trim(n.onclick); }
                    nodes[node_count++] = n;
                }
            }
        } else if (state == STATE_SCRIPT) { if (strncmp(p, "</script>", 9) == 0) { state = STATE_TEXT; p += 8; } }
        p++;
    }
}

void transpile_js_line(FILE* out, char* line) {
    trim(line); if (strlen(line) == 0) return;
    if (strncmp(line, "let ", 4) == 0) {
        char name[64], val[256];
        if (sscanf(line, "let %s = %[^;]", name, val) == 2) {
            if (isdigit(val[0])) fprintf(out, "    JSValue* %s = js_number(%s);\n", name, val);
            else { char sv[256]; sscanf(val, "'%[^']'", sv); fprintf(out, "    JSValue* %s = js_string(\"%s\");\n", name, sv); }
        }
    }
    else if (strstr(line, "console.log(")) {
        char var[64]; sscanf(line, "console.log(%[^)])", var); fprintf(out, "    js_console_log(%s);\n", var);
    }
    else if (strncmp(line, "function", 8) == 0) {
        char name[64]; sscanf(line, "function %s", name); char* p = strchr(name, '('); if(p) *p = '\0';
        fprintf(out, "void %s(Element* self) {\n", name);
    }
    else if (strcmp(line, "}") == 0) fprintf(out, "}\n");
    else if (strstr(line, " = ") && strchr(line, '+')) {
        char target[64], a[64], b[64];
        if (sscanf(line, "%s = %s + %[^;]", target, a, b) == 3) fprintf(out, "    %s = js_add(%s, %s);\n", target, a, b);
    }
    else if (strstr(line, ".innerText =")) {
        char id[64], val[64]; char* id_s = strchr(line, '\'') + 1; char* id_e = strchr(id_s, '\'');
        strncpy(id, id_s, id_e - id_s); id[id_e - id_s] = '\0';
        char* val_s = strrchr(line, ' ') + 1; char* endp = strchr(val_s, ';'); if(endp) *endp = '\0';
        fprintf(out, "    { Element* el = element_find_by_id(win->root, \"%s\"); if(el) element_set_text(el, js_to_str(%s)); }\n", id, val_s);
    }
}

void transpile(const char* i_f, const char* o_f) {
    FILE* in = fopen(i_f, "r"); FILE* out = fopen(o_f, "w"); if (!in || !out) return;
    fseek(in, 0, SEEK_END); long len = ftell(in); fseek(in, 0, SEEK_SET);
    char* buf = malloc(len + 1); fread(buf, 1, len, in); buf[len] = '\0'; fclose(in);
    fprintf(out, "#include \"engine.h\"\n#include \"js_runtime.h\"\n#include \"gc.h\"\n#include <stdio.h>\n\nWindow* win;\n");
    char* js_start = strstr(buf, "<script>"); char* js_end = strstr(buf, "</script>");
    if (js_start && js_end) {
        char* j_c = strndup(js_start + 8, js_end - js_start - 8); char* l_s = j_c, *l_e;
        while ((l_e = strchr(l_s, '\n'))) { *l_e = '\0'; transpile_js_line(out, l_s); l_s = l_e + 1; }
        transpile_js_line(out, l_s); free(j_c);
    }
    fprintf(out, "\nint main() {\n    engine_init();\n    win = window_create(800, 600, \"TPTC Compiler v1\");\n");
    parse_app(buf);
    for (int i = 0; i < node_count; i++) {
        fprintf(out, "    Element* el%d = element_create(%s, \"%s\");\n", i, nodes[i].type, nodes[i].id);
        if (strlen(nodes[i].content) > 0) fprintf(out, "    element_set_text(el%d, \"%s\");\n", i, nodes[i].content);
        fprintf(out, "    element_add_child(win->root, el%d);\n", i);
        if (strlen(nodes[i].onclick) > 0) fprintf(out, "    el%d->on_click = (void (*)(struct Element*))%s;\n", i, nodes[i].onclick);
    }
    fprintf(out, "    engine_run(win);\n    engine_terminate();\n    return 0;\n}\n");
    fclose(out); free(buf);
}
int main(int argc, char** argv) { if (argc < 3) return 1; transpile(argv[1], argv[2]); return 0; }
