#ifndef ENGINE_H
#define ENGINE_H
#include <stdbool.h>
#include "gc.h"
typedef enum { ELEMENT_TYPE_ROOT, ELEMENT_TYPE_BUTTON, ELEMENT_TYPE_TEXT } ElementType;
typedef struct Element {
    ElementType type; char *id, *value; float x, y, w, h;
    void (*on_click)(struct Element*); struct Element** children; int children_count;
} Element;
typedef struct { int width, height; const char* title; Element* root; } Window;
bool engine_init(); Window* window_create(int width, int height, const char* title);
void engine_run(Window* win); void engine_terminate();
Element* element_create(ElementType type, const char* id);
void element_add_child(Element* parent, Element* child);
void element_set_text(Element* el, const char* text);
Element* element_find_by_id(Element* root, const char* id);
#endif
