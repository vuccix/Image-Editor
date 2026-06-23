#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>
#include <ImGui/imgui_internal.h>
#include <cstdint>
#include <functional>

void UI::addFont(const auto& font, const float sizePixels) {
    const ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF(
        font.data(),
        font.size(),
        sizePixels
    );
}

void UI::dockspace(auto&& callback) {
    constexpr ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None     | ImGuiDockNodeFlags_NoResize
                                                 | ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoCloseButton;
    const     ImGuiViewport*     viewport        = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar               | ImGuiWindowFlags_NoDocking
                                            | ImGuiWindowFlags_NoTitleBar            | ImGuiWindowFlags_NoCollapse
                                            | ImGuiWindowFlags_NoResize              | ImGuiWindowFlags_NoMove
                                            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,   0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);

    std::invoke(std::forward<decltype(callback)>(callback));

    ImGui::End();
}

void UI::window(const char* name, auto&& body) {
    if (ImGui::Begin(name))
        body();

    ImGui::End();
}

void UI::window(const char* name, bool& open, auto&& body) {
    if (ImGui::Begin(name, &open))
        body();

    ImGui::End();
}

void UI::window(const char* name, const int flags, auto&& body) {
    if (ImGui::Begin(name, nullptr, flags))
        body();

    ImGui::End();
}

void UI::menuBar(auto&& body) {
    if (ImGui::BeginMenuBar()) {
        body();
        ImGui::EndMenuBar();
    }
}

void UI::menu(const char* name, auto&& body) {
    if (ImGui::BeginMenu(name)) {
        body();
        ImGui::EndMenu();
    }
}

void UI::item(const char* label, auto&& callback) {
    if (ImGui::MenuItem(label))
        std::invoke(std::forward<decltype(callback)>(callback));
}

void UI::item(const char* label, const bool isSelected, auto&& callback) {
    if (ImGui::MenuItem(label, nullptr, isSelected))
        std::invoke(std::forward<decltype(callback)>(callback));
}

void UI::item(const char* label, const char* shortcut, auto&& callback) {
    if (ImGui::MenuItem(label, shortcut))
        std::invoke(std::forward<decltype(callback)>(callback));
}

    ImGui::TextUnformatted(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);
    widget();
}

void UI::table(const char* label, const int columns, auto&& body) {
