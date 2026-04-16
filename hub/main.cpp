#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

int main() {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "TPTC HUB", NULL, NULL);
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

        ImGui::Text("TPTC - Hub de Proyectos");
        ImGui::Separator();

        if (ImGui::Button("CREAR PROYECTO", ImVec2(200, 60))) {
            // Logica para crear carpeta y archivos base
            system("mkdir -p MiProyectoNativo && printf '<html><body><h1>Hola</h1></body></html>' > MiProyectoNativo/index.html");
            system("touch MiProyectoNativo/styles.css MiProyectoNativo/main.js");
        }
        ImGui::SameLine();
        if (ImGui::Button("IMPORTAR PROYECTO", ImVec2(200, 60))) {
            ImGui::OpenPopup("ImportFromURL");
        }

        if (ImGui::BeginPopupModal("ImportFromURL", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char url_buf[512] = "";
            ImGui::Text("Pegue el enlace (GitHub, Drive o URL):");
            ImGui::InputText("##url", url_buf, sizeof(url_buf));
            if (ImGui::Button("DESCARGAR Y CONVERTIR", ImVec2(200, 40))) {
                char cmd[1024];
                sprintf(cmd, "./bin/downloader '%s' ./imported_project", url_buf);
                system(cmd);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancelar")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::Text("\nProyectos Recientes:");
        if (ImGui::Selectable("Mi Gran App Nativa", false, 0, ImVec2(0, 30))) {
            system("./bin/editor_gui &");
        }

        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    printf("\nTPTC HUB finalizado. Presione una tecla para cerrar...");
    getchar();
    return 0;
}
