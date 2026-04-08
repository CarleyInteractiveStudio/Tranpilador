#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { STATE_TEXT, STATE_TAG, STATE_SCRIPT, STATE_STYLE } ParserState;

typedef struct {
    char id[64]; char type[32]; char content[256]; char onclick[64]; char src[256];
    float r, g, b, a; float w, h, mt, ml;
    int has_color, has_size, has_margin, is_flex, flex_row;
} Node;

Node nodes[100]; int node_count = 0;

void trim(char* str) {
    char* end; while(isspace((unsigned char)*str)) str++; if(*str == 0) return;
    end = str + strlen(str) - 1; while(end > str && isspace((unsigned char)*end)) end--; end[1] = '\0';
}

void hex_to_rgb(const char* hex, float* r, float* g, float* b) {
    if (hex[0] == '#') hex++; unsigned int rgb; sscanf(hex, "%x", &rgb);
    *r = ((rgb >> 16) & 0xFF) / 255.0f; *g = ((rgb >> 8) & 0xFF) / 255.0f; *b = (rgb & 0xFF) / 255.0f;
}

void parse_css(const char* css) {
    char* copy = strdup(css); char* selector = strtok(copy, "{");
    while (selector) {
        char* properties = strtok(NULL, "}"); if (!properties) break;
        trim(selector);
        if (selector[0] == '#') {
            char id[64]; strcpy(id, selector + 1);
            for (int i=0; i<node_count; i++) {
                if (strcmp(nodes[i].id, id) == 0) {
                    char* prop;
                    if ((prop = strstr(properties, "background-color:"))) {
                        char hex[16]; sscanf(prop, "background-color: %[^;]", hex); trim(hex);
                        hex_to_rgb(hex, &nodes[i].r, &nodes[i].g, &nodes[i].b); nodes[i].a = 1.0f; nodes[i].has_color = 1;
                    }
                    if (strstr(properties, "display: flex")) nodes[i].is_flex = 1;
                    if (strstr(properties, "flex-direction: row")) nodes[i].flex_row = 1;
                    if ((prop = strstr(properties, "width:"))) { sscanf(prop, "width: %f", &nodes[i].w); nodes[i].has_size = 1; }
                    if ((prop = strstr(properties, "height:"))) { sscanf(prop, "height: %f", &nodes[i].h); nodes[i].has_size = 1; }
                }
            }
        }
        selector = strtok(NULL, "{");
    }
    free(copy);
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
                else if (strncmp(tag, "p", 1) == 0) { strcpy(n.type, "ELEMENT_TYPE_TEXT"); rec = 1; }
                if (rec) {
                    char* id_ptr = strstr(p, "id=\""); if (id_ptr) sscanf(id_ptr, "id=\"%[^\"]\"", n.id);
                    char* click_ptr = strstr(p, "onclick=\""); if (click_ptr) { sscanf(click_ptr, "onclick=\"%[^(]", n.onclick); trim(n.onclick); }
                    nodes[node_count++] = n;
                }
            }
        } else if (state == STATE_SCRIPT) { if (strncmp(p, "</script>", 9) == 0) { state = STATE_TEXT; p += 8; }
        } else if (state == STATE_STYLE) { if (strncmp(p, "</style>", 8) == 0) { state = STATE_TEXT; p += 7; }
        }
        p++;
    }
}

void transpile_js_line(FILE* out, char* line) {
    trim(line); if (strlen(line) == 0) return;
    if (strstr(line, "fetch(")) {
        char url[256], cb[64];
        if (sscanf(line, "fetch('%[^']').then(res => %s)", url, cb) == 2) {
             char* p = strchr(cb, '('); if(p) *p = '\0';
             fprintf(out, "    engine_http_get(\"%s\", (void (*)(const char*))%s);\n", url, cb);
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
        fprintf(out, "void %s(const char* response) {\n", name);
    } else if (strcmp(line, "}") == 0) fprintf(out, "}\n");
}

void transpile(const char* i_f, const char* o_f) {
    FILE* in = fopen(i_f, "r"); FILE* out = fopen(o_f, "w"); if (!in || !out) return;
    fseek(in, 0, SEEK_END); long len = ftell(in); fseek(in, 0, SEEK_SET);
    char* buf = malloc(len + 1); fread(buf, 1, len, in); buf[len] = '\0'; fclose(in);
    fprintf(out, "#include \"engine.h\"\n#include \"gc.h\"\n#include <stdio.h>\n\nWindow* win;\n");
    char* js_start = strstr(buf, "<script>"); char* js_end = strstr(buf, "</script>");
    if (js_start && js_end) {
        char* j_c = strndup(js_start + 8, js_end - js_start - 8); char* l_s = j_c, *l_e;
        while ((l_e = strchr(l_s, '\n'))) { *l_e = '\0'; transpile_js_line(out, l_s); l_s = l_e + 1; }
        transpile_js_line(out, l_s); free(j_c);
    }
    parse_app(buf); char* css_s = strstr(buf, "<style>"); char* css_e = strstr(buf, "</style>");
    if (css_s && css_e) { char* css_c = strndup(css_s+7, css_e-css_s-7); parse_css(css_c); free(css_c); }
    fprintf(out, "\nint main() {\n    engine_init();\n    win = window_create(800, 600, \"TPTC v0.5\");\n");
    for (int i = 0; i < node_count; i++) {
        fprintf(out, "    Element* el%d = element_create(%s, \"%s\");\n", i, nodes[i].type, nodes[i].id);
        if (nodes[i].has_color) fprintf(out, "    element_set_bg_color(el%d, %.2ff, %.2ff, %.2ff, 1.0f);\n", i, nodes[i].r, nodes[i].g, nodes[i].b);
        if (nodes[i].is_flex) fprintf(out, "    el%d->is_flex = true; el%d->flex_dir = %s;\n", i, i, nodes[i].flex_row ? "FLEX_DIR_ROW" : "FLEX_DIR_COLUMN");
        fprintf(out, "    element_add_child(win->root, el%d);\n", i);
        if (strlen(nodes[i].onclick) > 0) fprintf(out, "    el%d->on_click = (void (*)(struct Element*))%s;\n", i, nodes[i].onclick);
    }
    fprintf(out, "    engine_run(win);\n    engine_terminate();\n    return 0;\n}\n");
    fclose(out); free(buf);
}
int main(int argc, char** argv) { if (argc < 3) return 1; transpile(argv[1], argv[2]); return 0; }
