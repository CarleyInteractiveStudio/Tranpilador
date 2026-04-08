#include "js_runtime.h"
#include "engine.h"
#include <stdio.h>

// Librerias de sincronizacion DOM
JSValue* js_document_querySelector(JSValue* selector) {
    printf("[DOM API] querySelector: %s\n", js_to_str(selector));
    // En real buscaria en win->root usando el parser Lexbor
    return js_null();
}

void js_element_animate(JSValue* el_val, JSValue* keyframes) {
    printf("[DOM API] animate element with C native interpolation\n");
}
