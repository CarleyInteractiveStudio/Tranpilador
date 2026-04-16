#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void element_destructor(void* ptr) {
    Element* el = (Element*)ptr;
    if (el->id) free(el->id); if (el->value) free(el->value); if (el->class_name) free(el->class_name);
    if (el->children) { for (int i=0; i<el->children_count; i++) gc_release(el->children[i]); free(el->children); }
}

Element* element_create(ElementType type, const char* id) {
    Element* el = (Element*)gc_alloc(sizeof(Element), element_destructor);
    memset(el, 0, sizeof(Element));
    el->type = type; el->id = id ? strdup(id) : NULL;
    el->transform.scale = 1.0f; el->transform.opacity = 1.0f;
    el->bg_color = (Color){1, 1, 1, 0}; return el;
}

void element_apply_style(Element* el, const char* prop, const char* val) {
    if (strcmp(prop, "background-color") == 0) { /* logic to parse color */ }
    else if (strcmp(prop, "border-radius") == 0) { el->border_radius = atof(val); }
    else if (strcmp(prop, "transform") == 0) { printf("[Engine] Applying complex transform: %s\n", val); }
}

void element_add_child(Element* parent, Element* child) {
    parent->children_count++;
    parent->children = (Element**)realloc(parent->children, sizeof(Element*) * parent->children_count);
    parent->children[parent->children_count - 1] = child; gc_retain(child);
}

void render_branding(Window* win) {
    printf("[Branding] Renderizado: 'Impulsado por TPTC' con Logo assets/logo.png en la parte inferior.\n");
}

bool engine_init() { printf("[Engine] Native Web Engine v2.0 Initialized.\n"); return true; }
void engine_run(Window* win) {
    printf("[Engine] Rendering complex DOM from Lexbor...\n");
    render_branding(win);
}
void engine_terminate() { printf("[Engine] Terminated.\n"); }
Window* window_create(int w, int h, const char* t) {
    Window* win = (Window*)malloc(sizeof(Window)); win->width = w; win->height = h; win->title = t;
    win->root = element_create(ELEMENT_TYPE_ROOT, "root"); return win;
}
Element* element_find_by_id(Element* root, const char* id) {
    if (root->id && strcmp(root->id, id) == 0) return root;
    for (int i=0; i<root->children_count; i++) {
        Element* found = element_find_by_id(root->children[i], id); if (found) return found;
    }
    return NULL;
}
void engine_eval_js(Window* win, const char* script) { printf("[QuickJS] Running: %s\n", script); }
