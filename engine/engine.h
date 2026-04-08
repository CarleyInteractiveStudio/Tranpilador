#ifndef ENGINE_H
#define ENGINE_H
#include <stdbool.h>
#include "gc.h"

typedef struct { float r, g, b, a; } Color;

typedef enum { FLEX_DIR_ROW, FLEX_DIR_COLUMN } FlexDirection;
typedef enum { FLEX_JUSTIFY_START, FLEX_JUSTIFY_CENTER, FLEX_JUSTIFY_END, FLEX_JUSTIFY_BETWEEN } FlexJustify;

typedef enum { ELEMENT_TYPE_ROOT, ELEMENT_TYPE_BUTTON, ELEMENT_TYPE_TEXT, ELEMENT_TYPE_IMAGE, ELEMENT_TYPE_DIV } ElementType;

typedef struct Element {
    ElementType type;
    char *id, *value, *src;
    float x, y, w, h;
    float margin_top, margin_left, padding;

    // Flexbox
    bool is_flex;
    FlexDirection flex_dir;
    FlexJustify justify_content;

    Color bg_color, text_color;
    void (*on_click)(struct Element*);
    struct Element** children;
    int children_count;
} Element;

typedef struct { int width, height; const char* title; Element* root; } Window;

bool engine_init(); Window* window_create(int width, int height, const char* title);
void engine_run(Window* win); void engine_terminate();
Element* element_create(ElementType type, const char* id);
void element_add_child(Element* parent, Element* child);
void element_set_text(Element* el, const char* text);
void element_set_image(Element* el, const char* src);
void element_set_bg_color(Element* el, float r, float g, float b, float a);
void element_set_size(Element* el, float w, float h);
Element* element_find_by_id(Element* root, const char* id);

// Networking
void engine_http_get(const char* url, void (*callback)(const char* response));

#endif
