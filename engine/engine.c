#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Mocking OpenGL/GLFW for the sandbox, but structured for real use
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_QUADS 0x0007

void glClear(int mask) {}
void glClearColor(float r, float g, float b, float a) {}
void glBegin(int mode) {}
void glEnd() {}
void glVertex2f(float x, float y) {}
void glColor3f(float r, float g, float b) {}

typedef void* GLFWwindow;
int glfwInit() { return 1; }
void glfwTerminate() {}
GLFWwindow* glfwCreateWindow(int w, int h, const char* t, void* m, void* s) { return (GLFWwindow*)1; }
void glfwMakeContextCurrent(GLFWwindow* w) {}
int glfwWindowShouldClose(GLFWwindow* w) { return 0; }
void glfwSwapBuffers(GLFWwindow* w) {}
void glfwPollEvents() {}
void glfwWaitEvents() { /* En una app real esto esperaria */ }

void element_destructor(void* ptr) {
    Element* el = (Element*)ptr;
    if (el->id) free(el->id); if (el->value) free(el->value);
    if (el->children) { for (int i=0; i<el->children_count; i++) gc_release(el->children[i]); free(el->children); }
}

Element* element_create(ElementType type, const char* id) {
    Element* el = (Element*)gc_alloc(sizeof(Element), element_destructor);
    el->type = type; el->id = id ? strdup(id) : NULL; el->value = NULL;
    el->x = 50; el->y = 50; el->w = 200; el->h = 50; el->on_click = NULL;
    el->children = NULL; el->children_count = 0; return el;
}

void element_add_child(Element* parent, Element* child) {
    parent->children_count++;
    parent->children = (Element**)realloc(parent->children, sizeof(Element*) * parent->children_count);
    parent->children[parent->children_count - 1] = child; gc_retain(child);
}

void element_set_text(Element* el, const char* text) { if (el->value) free(el->value); el->value = strdup(text); }

Element* element_find_by_id(Element* root, const char* id) {
    if (root->id && strcmp(root->id, id) == 0) return root;
    for (int i=0; i<root->children_count; i++) {
        Element* found = element_find_by_id(root->children[i], id); if (found) return found;
    }
    return NULL;
}

bool engine_init() {
    printf("[Engine] Initializing OpenGL/GLFW...\n");
    return (bool)glfwInit();
}

Window* window_create(int width, int height, const char* title) {
    Window* win = (Window*)malloc(sizeof(Window));
    win->width = width; win->height = height; win->title = title;
    win->root = element_create(ELEMENT_TYPE_ROOT, "root");
    return win;
}

void render_element(Element* el) {
    if (el->type == ELEMENT_TYPE_BUTTON) {
        glColor3f(0.3f, 0.4f, 0.9f);
        glBegin(GL_QUADS);
            glVertex2f(el->x, el->y);
            glVertex2f(el->x + el->w, el->y);
            glVertex2f(el->x + el->w, el->y + el->h);
            glVertex2f(el->x, el->y + el->h);
        glEnd();
        printf("[OpenGL] Render Button '%s' ID: %s at (%.1f, %.1f)\n", el->value ? el->value : "", el->id, el->x, el->y);
    } else if (el->type == ELEMENT_TYPE_TEXT) {
        printf("[OpenGL] Render Text '%s' ID: %s at (%.1f, %.1f)\n", el->value ? el->value : "", el->id, el->x, el->y);
    }
    for (int i=0; i<el->children_count; i++) render_element(el->children[i]);
}

void engine_run(Window* win) {
    printf("[Engine] Starting Interactive Main Loop...\n");
    // Simulamos 3 frames para el sandbox, en real seria while(!glfwWindowShouldClose)
    for(int frame=0; frame<3; frame++) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render_element(win->root);

        // Simular click en el primer boton si existe (para probar interaccion en el sandbox)
        if (frame == 1 && win->root->children_count > 0 && win->root->children[0]->on_click) {
            printf("[Engine] Simulating Click on ID: %s\n", win->root->children[0]->id);
            win->root->children[0]->on_click(win->root->children[0]);
        }

        glfwSwapBuffers(NULL);
        glfwPollEvents();
    }
}

void engine_terminate() {
    printf("[Engine] Terminating OpenGL...\n");
    glfwTerminate();
}
