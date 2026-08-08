#include <View/UIManager.h>
#include "UI/UI.h"
#include "Utils.h"
#include <Model/EditorState.h>

void UIManager::drawToolbar(EditorState& state) {
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse
                                     | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
                                     | ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_AlwaysAutoResize;

    ui.window("Tools", flags, [&] {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

        ui.table("ToolbarGrid", 1, [&] {
            constexpr ImVec2 buttonSize(32, 32);
            constexpr std::array<const char*, 12> tools = {
                "V", "M",  // Move, Marquee
                "L", "W",  // Lasso, Magic Wand
                "C", "I",  // Crop, Eyedropper
                "B", "S",  // Brush, Stamp
                "E", "G",  // Eraser, Gradient
                "P", "T"   // Pen, Text
            };

            for (size_t i = 0; i < tools.size(); ++i) {
                ImGui::TableNextColumn();
                ImGui::PushID(static_cast<int>(i));

                if (ImGui::Button(tools[i], buttonSize)) {
                    // TODO: handle tool selection logic
                }

                ImGui::PopID();
            }

            ui.spacing();
            ui.separator();
            ui.spacing();

            constexpr ImGuiColorEditFlags colorFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;
            if (ImGui::ColorEdit4("Primary Color",   &state.primaryColor[0],   colorFlags)) {}
            if (ImGui::ColorEdit4("Secondary Color", &state.secondaryColor[0], colorFlags)) {}
        });

        ImGui::PopStyleVar();
    });
}
