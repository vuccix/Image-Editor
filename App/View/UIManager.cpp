#include <View/UIManager.h>
#include "UI/UI.h"
#include "UI/EditorThemes.h"
#include "Utils.h"
#include <Renderer/Renderer.h>
#include <Model/EditorState.h>
#include <algorithm>

UIManager::UIManager(Renderer& renderer) : m_renderer(renderer) {
    ui.init();
    setTheme(2);
    Utils::loadAssets(m_assets);
}

void UIManager::render(EditorState& state, Controller& controller, const std::function<void()>& onQuitRequest) {
    ui.beginFrame();
    ui.dockspace([&] {
        drawMenuBar(state, controller, onQuitRequest);
    });

    drawToolbar(state);
    drawPropertiesPanel(state, controller);
    drawCanvas(state);
    drawPendingPopups();

    ui.endFrame();
}

void UIManager::drawCanvas(EditorState& state) {
    ui.window("Canvas", [&] {
        ImDrawList* drawList     = ImGui::GetWindowDrawList();
        const ImGuiIO& io        = ImGui::GetIO();

        const ImVec2 windowPos   = ImGui::GetCursorScreenPos();
        const ImVec2 windowSize  = ImGui::GetContentRegionAvail();

        const auto windowCenter  = ImVec2(windowPos.x + windowSize.x * 0.5f,
                                          windowPos.y + windowSize.y * 0.5f);

        const float scaledWidth  = state.canvas.width()  * state.zoomLevel;
        const float scaledHeight = state.canvas.height() * state.zoomLevel;

        const auto canvasCenter  = ImVec2(windowCenter.x + state.panOffset.x,
                                          windowCenter.y + state.panOffset.y);
        const auto pMin          = ImVec2(canvasCenter.x - scaledWidth  * 0.5f,
                                          canvasCenter.y - scaledHeight * 0.5f);
        const auto pMax          = ImVec2(pMin.x + scaledWidth, pMin.y + scaledHeight);

        // handle inputs (zoom, pan, and touchpad) ---------------------------------------------------------------------
        if (ImGui::IsWindowHovered()) {
            const bool isCtrlDown  = ImGui::IsKeyDown(ImGuiKey_LeftCtrl)  || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
            const bool isShiftDown = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);

            // zoom
            if (io.MouseWheel != 0.f && isCtrlDown) {
                const float oldZoom        = state.zoomLevel;

                const float zoomFactor     = 1.f + (io.MouseWheel * 0.15f);
                state.zoomLevel            = std::clamp(state.zoomLevel * zoomFactor, 0.05f, 64.f);

                const float zoomRatio      = state.zoomLevel / oldZoom;

                const float mouseToCenterX = io.MousePos.x - canvasCenter.x;
                const float mouseToCenterY = io.MousePos.y - canvasCenter.y;

                state.panOffset.x          = io.MousePos.x - windowCenter.x - (mouseToCenterX * zoomRatio);
                state.panOffset.y          = io.MousePos.y - windowCenter.y - (mouseToCenterY * zoomRatio);
            }
            // scroll
            else if (constexpr float multiplier = 40.f;
                    !isCtrlDown && !isShiftDown) {

                if (io.MouseWheel != 0.f)
                    state.panOffset.y += io.MouseWheel * multiplier;

                if (io.MouseWheelH != 0.f)
                    state.panOffset.x += io.MouseWheelH * multiplier;
            }
            else if (!isCtrlDown) {
                if (io.MouseWheel != 0.f)
                    state.panOffset.x += io.MouseWheel * multiplier;
            }

            // pan
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
                state.panOffset.x += io.MouseDelta.x;
                state.panOffset.y += io.MouseDelta.y;
            }

            // reset zoom & pan
            if (isCtrlDown && ImGui::IsKeyDown(ImGuiKey_Keypad0)) {
                state.zoomLevel = 1.f;
                state.panOffset = { 0.f, 0.f };
            }
        }

        // draw checkerboard background --------------------------------------------------------------------------------
        constexpr float checkerSize = 16.f;
        const     auto  uvMax       = ImVec2(scaledWidth / checkerSize, scaledHeight / checkerSize);
        drawList->AddImage(static_cast<ImTextureID>(static_cast<intptr_t>(m_assets.get(Asset::Checkerboard))),
                           pMin, pMax, ImVec2(0, 0), uvMax);

        // draw canvas composite ---------------------------------------------------------------------------------------
        drawList->AddImage(static_cast<ImTextureID>(static_cast<intptr_t>(m_renderer.textureID())), pMin, pMax);

        // draw canvas border ------------------------------------------------------------------------------------------
        const ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
        drawList->AddRect(pMin, pMax, borderColor, 0.f, ImDrawFlags_None, 2.f);

        // status bar --------------------------------------------------------------------------------------------------
        {
            uint32_t canvasX = 0;
            uint32_t canvasY = 0;

            if (ImGui::IsWindowHovered()) {
                canvasX = static_cast<uint32_t>((io.MousePos.x - pMin.x) / state.zoomLevel);
                canvasY = static_cast<uint32_t>((io.MousePos.y - pMin.y) / state.zoomLevel);
            }

            // status bar position
            ImGui::SetCursorScreenPos(ImVec2(windowPos.x, windowPos.y + windowSize.y - 25.f));

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(ImGuiCol_TableHeaderBg, 0.7f));
            if (ImGui::BeginChild("StatusBar", ImVec2(250.f, 24.f), ImGuiChildFlags_Borders,
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
                ImGui::Text(" X: %d px | Y: %d px | Zoom: %.0f%%", canvasX, canvasY, state.zoomLevel * 100.f);

            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
    });
}

void UIManager::drawPendingPopups() {
    if (!m_activePopup.has_value())
        return;

    ImGui::OpenPopup(m_activePopup->title.data());

    // center modal
    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.f, 0.f, 0.f, 0.f));

    if (ImGui::BeginPopupModal(m_activePopup->title.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        m_activePopup->drawContent();

        ui.spacing();
        ui.separator();

        constexpr float   buttonWidth = 120.f;
        constexpr int32_t buttonCount = 2;
        const     float   itemSpacing = ImGui::GetStyle().ItemSpacing.x;

        const float totalButtonsWidth = (buttonWidth * buttonCount) + itemSpacing;
        const float availWidth        = ImGui::GetContentRegionAvail().x;

        if (const float startPosX = (availWidth - totalButtonsWidth) * 0.5f; startPosX > 0.f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + startPosX);

        // control buttons
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0.f))) {
            ImGui::CloseCurrentPopup();
            m_activePopup.reset(); // clear state
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply", ImVec2(buttonWidth, 0.f))) {
            m_activePopup->onApply();
            ImGui::CloseCurrentPopup();
            m_activePopup.reset(); // clear state
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleColor();
}

void UIManager::setTheme(const uint8_t theme) {
    m_theme = theme;

    switch (m_theme) {
        case  0: Themes::classicImGuiTheme(); break;
        case  1: Themes::darkImGuiTheme();    break;
        case  2: Themes::lightImGuiTheme();   break;
        case  3: Themes::classicValveTheme(); break;
        case  4: Themes::draculaTheme();      break;
        case  5: Themes::discordTheme();      break;
        case  6: Themes::enemyMouseTheme();   break;
        case  7: Themes::greenTheme();        break;
        case  8: Themes::photoshopTheme();    break;
        case  9: Themes::visualStudioTheme(); break;
        case 10: Themes::progtestTheme();     break;
        default: break;
    }
}
