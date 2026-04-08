#include "js_runtime.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void js_value_destructor(void* ptr) {
    JSValue* v = (JSValue*)ptr;
    if (v->type == JS_TYPE_STRING && v->s) free(v->s);
}

JSValue* js_alloc() { return (JSValue*)gc_alloc(sizeof(JSValue), js_value_destructor); }

JSValue* js_null() { JSValue* v = js_alloc(); v->type = JS_TYPE_NULL; return v; }
JSValue* js_bool(bool b) { JSValue* v = js_alloc(); v->type = JS_TYPE_BOOL; v->b = b; return v; }
JSValue* js_number(double n) { JSValue* v = js_alloc(); v->type = JS_TYPE_NUMBER; v->n = n; return v; }
JSValue* js_string(const char* s) { JSValue* v = js_alloc(); v->type = JS_TYPE_STRING; v->s = strdup(s); return v; }

JSValue* js_add(JSValue* a, JSValue* b) {
    if (a->type == JS_TYPE_NUMBER && b->type == JS_TYPE_NUMBER) return js_number(a->n + b->n);
    // Concatenación de strings simplificada
    char buf[512];
    sprintf(buf, "%s%s", js_to_str(a), js_to_str(b));
    return js_string(buf);
}

const char* js_to_str(JSValue* v) {
    static char buf[64];
    if (v->type == JS_TYPE_NUMBER) { sprintf(buf, "%.2f", v->n); return buf; }
    if (v->type == JS_TYPE_STRING) return v->s;
    if (v->type == JS_TYPE_BOOL) return v->b ? "true" : "false";
    return "null";
}

void js_console_log(JSValue* v) { printf("[JS Console] %s\n", js_to_str(v)); }
