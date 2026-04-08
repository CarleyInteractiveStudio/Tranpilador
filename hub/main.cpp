#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

int main(int, char**) {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "TPTC HUB", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("HUB", NULL, ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("TPTC HUB - Gestion de Proyectos");
        if (ImGui::Button("Abrir TPTC Editor", ImVec2(200, 50))) {
            system("./bin/editor_gui &");
        }
        if (ImGui::Button("Salir", ImVec2(200, 50))) glfwSetWindowShouldClose(window, true);
        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
