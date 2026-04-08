#ifndef GC_H
#define GC_H
#include <stdlib.h>
typedef struct { int ref_count; void (*destructor)(void*); } GCHeader;
void* gc_alloc(size_t size, void (*destructor)(void*));
void gc_retain(void* ptr);
void gc_release(void* ptr);
#endif
