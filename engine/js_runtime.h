#ifndef JS_RUNTIME_H
#define JS_RUNTIME_H

#include <stdbool.h>
#include "gc.h"

typedef enum { JS_TYPE_NULL, JS_TYPE_BOOL, JS_TYPE_NUMBER, JS_TYPE_STRING, JS_TYPE_OBJECT } JSType;

typedef struct JSValue {
    JSType type;
    union {
        bool b;
        double n;
        char* s;
        void* obj;
    };
} JSValue;

JSValue* js_null();
JSValue* js_bool(bool b);
JSValue* js_number(double n);
JSValue* js_string(const char* s);

JSValue* js_add(JSValue* a, JSValue* b);
bool js_to_bool(JSValue* v);
const char* js_to_str(JSValue* v);

// Browser Libs
void js_console_log(JSValue* v);

#endif
