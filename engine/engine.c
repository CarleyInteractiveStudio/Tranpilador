#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simulación de QuickJS para el Sandbox (En real seria quickjs.h)
void* JS_NewRuntime() { return (void*)1; }
void* JS_NewContext(void* rt) { return (void*)2; }
void JS_Eval(void* ctx, const char* script) { printf("[QuickJS] Evaluando: %s\n", script); }

void element_destructor(void* ptr) {
    Element* el = (Element*)ptr;
    if (el->id) free(el->id); if (el->value) free(el->value); if (el->js_onclick) free(el->js_onclick);
    if (el->children) { for (int i=0; i<el->children_count; i++) gc_release(el->children[i]); free(el->children); }
}

Element* element_create(ElementType type, const char* id) {
    Element* el = (Element*)gc_alloc(sizeof(Element), element_destructor);
    el->type = type; el->id = id ? strdup(id) : NULL; el->value = NULL; el->js_onclick = NULL;
    el->x = 0; el->y = 0; el->w = 100; el->h = 40; el->mt = 0; el->ml = 0; el->pad = 0;
    el->is_flex = false; el->flex_dir = FLEX_DIR_COLUMN;
    el->bg_color = (Color){0.9f, 0.9f, 0.9f, 1.0f};
    el->children = NULL; el->children_count = 0; return el;
}

void element_add_child(Element* parent, Element* child) {
    parent->children_count++;
    parent->children = (Element**)realloc(parent->children, sizeof(Element*) * parent->children_count);
    parent->children[parent->children_count - 1] = child; gc_retain(child);
}

void element_set_text(Element* el, const char* text) { if (el->value) free(el->value); el->value = strdup(text); }
void element_set_bg_color(Element* el, float r, float g, float b, float a) { el->bg_color = (Color){r, g, b, a}; }

Element* element_find_by_id(Element* root, const char* id) {
    if (root->id && strcmp(root->id, id) == 0) return root;
    for (int i=0; i<root->children_count; i++) {
        Element* found = element_find_by_id(root->children[i], id); if (found) return found;
    }
    return NULL;
}

void engine_eval_js(Window* win, const char* script) { JS_Eval(win->js_context, script); }

void engine_storage_set(const char* key, const char* val) { printf("[Storage] SAVE %s = %s\n", key, val); }
const char* engine_storage_get(const char* key) { printf("[Storage] LOAD %s\n", key); return "null"; }

bool engine_init() { setvbuf(stdout, NULL, _IONBF, 0); printf("[Engine] Initializing v0.6 (QuickJS & Storage)...\n"); return true; }

Window* window_create(int width, int height, const char* title) {
    Window* win = (Window*)malloc(sizeof(Window)); win->width = width; win->height = height; win->title = title;
    win->js_runtime = JS_NewRuntime(); win->js_context = JS_NewContext(win->js_runtime);
    win->root = element_create(ELEMENT_TYPE_ROOT, "root"); return win;
}

void render_element(Element* el, float px, float py) {
    float cx = px + el->ml; float cy = py + el->mt;
    if (el->type != ELEMENT_TYPE_ROOT) {
        printf("[Render] %s ID: %s at (%.1f, %.1f) Style: Animation Interpolated\n",
            el->type == ELEMENT_TYPE_BUTTON ? "Button" : "Input", el->id, cx, cy);
    }
    float sx = cx + el->pad; float sy = cy + el->pad;
    for (int i=0; i<el->children_count; i++) {
        render_element(el->children[i], sx, sy);
        if (el->is_flex && el->flex_dir == FLEX_DIR_ROW) sx += el->children[i]->w + el->children[i]->ml;
        else sy += el->children[i]->h + el->children[i]->mt;
    }
}

void engine_run(Window* win) {
    printf("[Engine] Running Application Cycle...\n");
    render_element(win->root, 0, 0);
    // Simular un evento que dispara JS
    if (win->root->children_count > 0 && win->root->children[0]->js_onclick) {
        printf("[Engine] Event: Click -> JS Execute\n");
        engine_eval_js(win, win->root->children[0]->js_onclick);
    }
}
void engine_terminate() { printf("[Engine] Cleanup QuickJS and Terminate.\n"); }
