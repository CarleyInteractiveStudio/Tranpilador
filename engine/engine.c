#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GL_COLOR_BUFFER_BIT 0x00004000
void glClear(int mask) {}
void glClearColor(float r, float g, float b, float a) {}
void glColor4f(float r, float g, float b, float a) {}

void element_destructor(void* ptr) {
    Element* el = (Element*)ptr;
    if (el->id) free(el->id); if (el->value) free(el->value); if (el->src) free(el->src);
    if (el->children) { for (int i=0; i<el->children_count; i++) gc_release(el->children[i]); free(el->children); }
}

Element* element_create(ElementType type, const char* id) {
    Element* el = (Element*)gc_alloc(sizeof(Element), element_destructor);
    el->type = type; el->id = id ? strdup(id) : NULL; el->value = NULL; el->src = NULL;
    el->x = 0; el->y = 0; el->w = 100; el->h = 50;
    el->is_flex = false; el->flex_dir = FLEX_DIR_COLUMN; el->justify_content = FLEX_JUSTIFY_START;
    el->bg_color = (Color){1.0f, 1.0f, 1.0f, 0.0f};
    el->on_click = NULL; el->children = NULL; el->children_count = 0; return el;
}

void element_add_child(Element* parent, Element* child) {
    parent->children_count++;
    parent->children = (Element**)realloc(parent->children, sizeof(Element*) * parent->children_count);
    parent->children[parent->children_count - 1] = child; gc_retain(child);
}

void element_set_text(Element* el, const char* text) { if (el->value) free(el->value); el->value = strdup(text); }
void element_set_image(Element* el, const char* src) { if (el->src) free(el->src); el->src = strdup(src); }
void element_set_bg_color(Element* el, float r, float g, float b, float a) { el->bg_color = (Color){r, g, b, a}; }
void element_set_size(Element* el, float w, float h) { el->w = w; el->h = h; }

Element* element_find_by_id(Element* root, const char* id) {
    if (root->id && strcmp(root->id, id) == 0) return root;
    for (int i=0; i<root->children_count; i++) {
        Element* found = element_find_by_id(root->children[i], id); if (found) return found;
    }
    return NULL;
}

void engine_http_get(const char* url, void (*callback)(const char* response)) {
    printf("[Network] GET Request to: %s\n", url);
    if (callback) callback("{\"status\": \"ok\", \"data\": \"Simulated API response\"}");
}

bool engine_init() { setvbuf(stdout, NULL, _IONBF, 0); printf("[Engine] Initializing Version 0.5 (Flexbox & Network)...\n"); return true; }

void render_element(Element* el, float parent_x, float parent_y) {
    float cur_x = parent_x + el->margin_left;
    float cur_y = parent_y + el->margin_top;
    el->x = cur_x; el->y = cur_y;

    if (el->type != ELEMENT_TYPE_ROOT) {
        printf("[Render] %s ID: %s at (%.1f, %.1f) Color: (%.1f,%.1f,%.1f)\n",
            el->type == ELEMENT_TYPE_BUTTON ? "Button" : (el->type == ELEMENT_TYPE_DIV ? "Div" : "Text"),
            el->id ? el->id : "none", el->x, el->y, el->bg_color.r, el->bg_color.g, el->bg_color.b);
    }

    float child_x = cur_x + el->padding;
    float child_y = cur_y + el->padding;

    for (int i=0; i<el->children_count; i++) {
        render_element(el->children[i], child_x, child_y);
        if (el->is_flex && el->flex_dir == FLEX_DIR_ROW) {
            child_x += el->children[i]->w + el->children[i]->margin_left;
        } else {
            child_y += el->children[i]->h + el->children[i]->margin_top;
        }
    }
}

void engine_run(Window* win) {
    printf("[Engine] Running...\n");
    render_element(win->root, 0, 0);
    if (win->root->children_count > 0 && win->root->children[0]->on_click) {
        printf("[Engine] Event: Click on %s\n", win->root->children[0]->id);
        win->root->children[0]->on_click(win->root->children[0]);
    }
}
void engine_terminate() { printf("[Engine] Terminate.\n"); }

Window* window_create(int width, int height, const char* title) {
    Window* win = (Window*)malloc(sizeof(Window)); win->width = width; win->height = height; win->title = title;
    win->root = element_create(ELEMENT_TYPE_ROOT, "root"); return win;
}
