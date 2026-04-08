#ifndef ENGINE_H
#define ENGINE_H
#include <stdbool.h>
#include "gc.h"

typedef struct { float r, g, b, a; } Color;
typedef enum { FLEX_DIR_ROW, FLEX_DIR_COLUMN } FlexDirection;
typedef enum { ELEMENT_TYPE_ROOT, ELEMENT_TYPE_BUTTON, ELEMENT_TYPE_TEXT, ELEMENT_TYPE_DIV, ELEMENT_TYPE_INPUT } ElementType;

typedef struct Element {
    ElementType type;
    char *id, *value;
    float x, y, w, h;
    float mt, ml, pad;
    bool is_flex;
    FlexDirection flex_dir;
    Color bg_color;
    char* js_onclick; // El script a ejecutar al hacer click
    struct Element** children;
    int children_count;
} Element;

typedef struct { int width, height; const char* title; Element* root; void* js_runtime; void* js_context; } Window;

bool engine_init(); Window* window_create(int width, int height, const char* title);
void engine_run(Window* win); void engine_terminate();
Element* element_create(ElementType type, const char* id);
void element_add_child(Element* parent, Element* child);
void element_set_text(Element* el, const char* text);
void element_set_bg_color(Element* el, float r, float g, float b, float a);
Element* element_find_by_id(Element* root, const char* id);

// JS Integration
void engine_eval_js(Window* win, const char* script);

// Storage
void engine_storage_set(const char* key, const char* val);
const char* engine_storage_get(const char* key);

#endif
