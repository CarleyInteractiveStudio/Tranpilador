#ifndef ENGINE_H
#define ENGINE_H
#include <stdbool.h>
#include "gc.h"

typedef struct { float r, g, b, a; } Color;
typedef enum { FLEX_DIR_ROW, FLEX_DIR_COLUMN } FlexDirection;
typedef enum { ELEMENT_TYPE_ROOT, ELEMENT_TYPE_DIV, ELEMENT_TYPE_BUTTON, ELEMENT_TYPE_TEXT, ELEMENT_TYPE_IMAGE, ELEMENT_TYPE_INPUT } ElementType;

typedef struct {
    float x, y, z;
    float scale;
    float opacity;
} Transform;

typedef struct Element {
    ElementType type;
    char *id, *class_name, *value, *src;
    float x, y, w, h;
    float mt, ml, mr, mb, pad;
    bool is_flex;
    FlexDirection flex_dir;
    Color bg_color, text_color, border_color;
    float border_width, border_radius;
    Transform transform;
    char* js_onclick;
    struct Element** children;
    int children_count;
} Element;

typedef struct { int width, height; const char* title; Element* root; void* js_ctx; } Window;

bool engine_init(); Window* window_create(int width, int height, const char* title);
void engine_run(Window* win); void engine_terminate();
Element* element_create(ElementType type, const char* id);
void element_add_child(Element* parent, Element* child);
void element_apply_style(Element* el, const char* prop, const char* val);
Element* element_find_by_id(Element* root, const char* id);
void engine_eval_js(Window* win, const char* script);

#endif
