#include "gc.h"
#include <stdlib.h>
void* gc_alloc(size_t size, void (*destructor)(void*)) {
    GCHeader* header = (GCHeader*)malloc(sizeof(GCHeader) + size);
    header->ref_count = 1; header->destructor = destructor;
    return (void*)(header + 1);
}
void gc_retain(void* ptr) { if (ptr) { ((GCHeader*)ptr - 1)->ref_count++; } }
void gc_release(void* ptr) {
    if (!ptr) return;
    GCHeader* header = (GCHeader*)ptr - 1;
    if (--header->ref_count <= 0) { if (header->destructor) header->destructor(ptr); free(header); }
}
