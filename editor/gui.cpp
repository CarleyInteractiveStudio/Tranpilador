#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

// Mock de iconos
#define ICON_FILE "[F]"
#define ICON_FOLDER "[D]"
#define ICON_GEAR "[*]"

struct Theme {
    ImVec4 bg;
    ImVec4 text;
    ImVec4 accent;
};

Theme dark_theme = { ImVec4(0.1f, 0.1f, 0.1f, 1.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f), ImVec4(0.2f, 0.5f, 0.8f, 1.0f) };

void apply_style(Theme t) {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = t.bg;
    style.Colors[ImGuiCol_Text] = t.text;
    style.Colors[ImGuiCol_Button] = t.accent;
}

int main() {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1600, 900, "TPTC Professional IDE", NULL, NULL);
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char html_buffer[8192] = "<!-- HTML Source -->\n<div id='app'>\n  <h1>Hola Nativo</h1>\n</div>";
    char css_buffer[4096] = "/* CSS Styles */\n#app { background-color: #222; }";
    char js_buffer[8192] = "// JS Logic\nconsole.log('App ready');";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // BARRA SUPERIOR
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Archivos")) {
                if (ImGui::BeginMenu("Nuevo Archivo")) {
                    if (ImGui::MenuItem("JavaScript (.js)")) {}
                    if (ImGui::MenuItem("HTML (.html)")) {}
                    if (ImGui::MenuItem("CSS (.css)")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Importar Archivo (Imagen/SVG)")) {}
                if (ImGui::MenuItem("Importar Proyecto (Carpeta)")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Abrir Nuevo Proyecto")) {}
                if (ImGui::MenuItem("Cerrar Proyecto")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editar")) {
                if (ImGui::BeginMenu("Configurar Tema")) {
                    if (ImGui::MenuItem("Tema Oscuro")) { apply_style(dark_theme); }
                    if (ImGui::MenuItem("Tema Claro")) {}
                    if (ImGui::MenuItem("Personalizar Colores")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150);
            if (ImGui::BeginMenu("Exportar")) {
                ImGui::TextDisabled("Paso 1:");
                if (ImGui::MenuItem("Compilar a C Nativo")) {}
                ImGui::Separator();
                ImGui::TextDisabled("Paso 2 (Exportar):");
                if (ImGui::MenuItem("Windows (.exe)")) {}
                if (ImGui::MenuItem("Android (.apk)")) {}
                if (ImGui::MenuItem("iOS (.ipa)")) {}
                if (ImGui::MenuItem("Linux (.bin)")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // SIDEBAR IZQUIERDA
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (ImGui::CollapsingHeader(ICON_GEAR " Extensiones")) {
            ImGui::Text("TCC Native Compiler [v1.0]");
            ImGui::Text("HTML5 Parser [Lexbor]");
        }

        if (ImGui::CollapsingHeader(ICON_FOLDER " Archivos del Proyecto")) {
            if (ImGui::TreeNode("Archivos")) {
                ImGui::Text(ICON_FILE " index.html");
                ImGui::Text(ICON_FILE " styles.css");
                ImGui::Text(ICON_FILE " main.js");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Resultado")) {
                if (ImGui::TreeNode("Codigos C")) {
                    ImGui::Text(ICON_FILE " generated_app.c");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Ejecutables")) {
                    ImGui::Text(ICON_FILE " app.exe");
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();

        // EDITOR CENTRAL
        ImGui::SetNextWindowPos(ImVec2(300, 20));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 150, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Begin("Editor", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if (ImGui::BeginTabBar("EditorTabs")) {
            if (ImGui::BeginTabItem("index.html")) {
                ImGui::InputTextMultiline("##html", html_buffer, sizeof(html_buffer), ImVec2(-FLT_MIN, -FLT_MIN));
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("styles.css")) {
                ImGui::InputTextMultiline("##css", css_buffer, sizeof(css_buffer), ImVec2(-FLT_MIN, -FLT_MIN));
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("main.js")) {
                ImGui::InputTextMultiline("##js", js_buffer, sizeof(js_buffer), ImVec2(-FLT_MIN, -FLT_MIN));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        // PREVIEW DERECHA
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 + 150, 20));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 150, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Begin("Live Preview", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "VISTA PREVIA EN TIEMPO REAL");
        ImGui::Separator();
        ImGui::BeginChild("PreviewContent", ImVec2(0, 0), true);
        ImGui::Text("Simulando Renderizado Nativo...");
        ImGui::Button("Boton desde HTML");
        ImGui::EndChild();
        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
