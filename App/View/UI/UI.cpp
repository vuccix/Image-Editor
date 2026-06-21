#include "UI.h"
#include "Fonts/font_roboto.hf"
#include "Fonts/font_roboto_bold.hf"
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <cstdarg>

#include "GLFW/glfw3.h"

void UI::init() {
    if (m_isInit) return;
    m_isInit = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io     = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // enable docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // allow dragging panels outside main window

    // setup bindings
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // setup fonts
    ui.addFont(Roboto_Font_data,      18.f);
    ui.addFont(Roboto_Bold_Font_data, 18.f);
}

void UI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // handle multi-viewports
    // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //     GLFWwindow* context = glfwGetCurrentContext();
    //     ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    //     glfwMakeContextCurrent(context);
    // }
}

void UI::separator() {
    ImGui::Separator();
}

void UI::spacing() {
    ImGui::Spacing();
}

UI ui;
