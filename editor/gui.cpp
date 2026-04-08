#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

int main(int, char**) {
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR_0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "TPTC NATIVE IDE", NULL, NULL);
    if (window == NULL) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_demo_window = true;
    char code_buffer[4096] = "<html>\n  <button id=\"btn1\">Haz click</button>\n  <p id=\"txt1\">Texto original</p>\n</html>\n\n<script>\nfunction cambiarTexto() {\n  document.getElementById('txt1').innerText = '¡Texto cambiado!'\n}\n</script>";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI Principal
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(io.DisplaySize);
            ImGui::Begin("TPTC IDE", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Archivo")) {
                    if (ImGui::MenuItem("Nuevo")) {}
                    if (ImGui::MenuItem("Abrir")) {}
                    if (ImGui::MenuItem("Salir")) { glfwSetWindowShouldClose(window, true); }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Columns(2, "MainColumns");

            // Columna Izquierda: Editor de Código
            ImGui::Text("Editor de Código (.tn)");
            ImGui::InputTextMultiline("##source", code_buffer, IM_ARRAYSIZE(code_buffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 30));

            if (ImGui::Button("GUARDAR")) {
                FILE* f = fopen("app.tn", "w");
                fputs(code_buffer, f);
                fclose(f);
            }
            ImGui::SameLine();
            if (ImGui::Button("COMPILAR Y EXPORTAR NATIVO")) {
                // Llamada al sistema para compilar
                system("make app.exe");
            }

            ImGui::NextColumn();

            // Columna Derecha: Vista Previa y Logs
            ImGui::Text("Consola de Compilación");
            ImGui::BeginChild("LogWindow", ImVec2(0, 0), true);
            ImGui::TextWrapped("Compilador listo. Esperando acciones...");
            ImGui::EndChild();

            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
