#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { STATE_TEXT, STATE_TAG, STATE_SCRIPT, STATE_STYLE } ParserState;

typedef struct {
    char id[64]; char type[32]; char content[256]; char onclick[1024];
    float r, g, b; float w, h, mt, ml;
    int has_color, has_size, is_flex, flex_row;
} Node;

Node nodes[200]; int node_count = 0;
char global_js[10000] = {0};

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
                else if (strncmp(p, "<style>", 7) == 0) { state = STATE_STYLE; p += 6; }
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
                else if (strncmp(tag, "div", 3) == 0) { strcpy(n.type, "ELEMENT_TYPE_DIV"); rec = 1; p += 2; }
                else if (strncmp(tag, "input", 5) == 0) { strcpy(n.type, "ELEMENT_TYPE_INPUT"); rec = 1; p += 4; }
                else if (strncmp(tag, "p", 1) == 0) { strcpy(n.type, "ELEMENT_TYPE_TEXT"); rec = 1; }
                if (rec) {
                    char* id_ptr = strstr(p, "id=\""); if (id_ptr) sscanf(id_ptr, "id=\"%[^\"]\"", n.id);
                    char* click_ptr = strstr(p, "onclick=\""); if (click_ptr) sscanf(click_ptr, "onclick=\"%[^\"]\"", n.onclick);
                    nodes[node_count++] = n;
                }
            }
        } else if (state == STATE_SCRIPT) {
            if (strncmp(p, "</script>", 9) == 0) { state = STATE_TEXT; p += 8; }
            else { strncat(global_js, p, 1); }
        } else if (state == STATE_STYLE) { if (strncmp(p, "</style>", 8) == 0) { state = STATE_TEXT; p += 7; }
        }
        p++;
    }
}

void transpile(const char* i_f, const char* o_f) {
    FILE* in = fopen(i_f, "r"); FILE* out = fopen(o_f, "w"); if (!in || !out) return;
    fseek(in, 0, SEEK_END); long len = ftell(in); fseek(in, 0, SEEK_SET);
    char* buf = malloc(len + 1); fread(buf, 1, len, in); buf[len] = '\0'; fclose(in);
    parse_app(buf);
    fprintf(out, "#include \"engine.h\"\n#include \"gc.h\"\n#include <stdio.h>\n\n");
    fprintf(out, "int main() {\n    engine_init();\n    Window* win = window_create(800, 600, \"TPTC v0.6 JS-Engine\");\n");
    // Inyectar el JS global en el motor al iniciar
    if (strlen(global_js) > 0) {
        fprintf(out, "    engine_eval_js(win, \" %s \");\n", global_js);
    }
    for (int i = 0; i < node_count; i++) {
        fprintf(out, "    Element* el%d = element_create(%s, \"%s\");\n", i, nodes[i].type, nodes[i].id);
        if (strlen(nodes[i].content) > 0) fprintf(out, "    element_set_text(el%d, \"%s\");\n", i, nodes[i].content);
        if (strlen(nodes[i].onclick) > 0) {
            fprintf(out, "    el%d->js_onclick = strdup(\"%s\");\n", i, nodes[i].onclick);
        }
        fprintf(out, "    element_add_child(win->root, el%d);\n", i);
    }
    fprintf(out, "    engine_run(win);\n    engine_terminate();\n    return 0;\n}\n");
    fclose(out); free(buf);
}
int main(int argc, char** argv) { if (argc < 3) return 1; transpile(argv[1], argv[2]); return 0; }
