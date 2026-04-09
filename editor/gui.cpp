#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

// Iconos estilizados para el lanzamiento
#define ICON_HTML "<H>"
#define ICON_CSS  "{#}"
#define ICON_JS   "(J)"
#define ICON_IMG  "[I]"
#define ICON_FOLDER " > "

int main() {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1600, 900, "TPTC ULTIMATE IDE", NULL, NULL);
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char html_code[8192] = "<html>\n  <body>\n    <h1>Lanzamiento TPTC</h1>\n  </body>\n</html>";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Barra Superior
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Archivos")) { ImGui::EndMenu(); }
            if (ImGui::BeginMenu("Exportar")) {
                if (ImGui::MenuItem("Paso 1: Compilar Proyecto")) {}
                if (ImGui::MenuItem("Paso 2: Generar APK/EXE")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Sidebar con Iconos
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Begin("Filesystem", NULL, ImGuiWindowFlags_NoTitleBar);
        ImGui::TextColored(ImVec4(1,1,0,1), "EXPLORADOR PRO");
        ImGui::Separator();
        if (ImGui::TreeNodeEx("Mi Proyecto", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::TextColored(ImVec4(1,0.5f,0,1), ICON_HTML " index.html");
            ImGui::TextColored(ImVec4(0,0.5f,1,1), ICON_CSS  " styles.css");
            ImGui::TextColored(ImVec4(1,1,0,1), ICON_JS   " main.js");
            ImGui::TextColored(ImVec4(0,1,0,1), ICON_IMG  " logo.png");
            ImGui::TreePop();
        }
        ImGui::End();

        // Editor y Preview (Simplificado para el renderizado final)
        ImGui::SetNextWindowPos(ImVec2(300, 20));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 300, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Begin("MainView", NULL, ImGuiWindowFlags_NoTitleBar);
        ImGui::Columns(2);
        ImGui::Text("EDITOR");
        ImGui::InputTextMultiline("##editor", html_code, sizeof(html_code), ImVec2(-FLT_MIN, -FLT_MIN));
        ImGui::NextColumn();
        ImGui::Text("VISTA PREVIA");
        ImGui::BeginChild("Render", ImVec2(0,0), true);
        ImGui::Button("Boton Nativo Test");
        ImGui::EndChild();
        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
