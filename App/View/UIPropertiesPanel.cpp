#include <View/UIManager.h>
#include "UI/UI.h"
#include "Utils.h"
#include <Renderer/Renderer.h>
#include <Model/EditorState.h>
#include <Controller/Controller.h>
#include <Controller/Commands/CanvasCommand.h>
#include <Controller/Commands/FunctionalCommand.h>
#include <Controller/Commands/StatefulCommand.h>

void UIManager::drawPropertiesPanel(EditorState& state, Controller& controller) {
    assert(state.canvas.layerCount() > 0);

    auto exec = [&controller, &state](std::unique_ptr<Command>&& func) {
        controller.execute(state, std::move(func));
    };

    ui.window("Layers", [&] {
        Canvas& canvas          = state.canvas;
        const size_t layerCount = canvas.layerCount();
        bool hasChanged         = false;

        if (state.selectedLayerID >= layerCount)
            state.selectedLayerID = static_cast<uint32_t>(layerCount - 1);

        // selected layer properties ===================================================================================
        {
            ImGui::PushID("SelectedLayerControls");

            Layer& selectedLayer = canvas[state.selectedLayerID];

            // blending mode dropdown
            constexpr std::array blendModes = { "Normal", "Multiply", "Screen", "Overlay", "Darken", "Lighten" };
            static int32_t currentBlendMode = 0; // placeholder state

            if (ImGui::Combo("##BlendMode", &currentBlendMode, blendModes.data(), blendModes.size())) {
                // selectedLayer.blendMode = currentBlendMode;
            }

            ImGui::SameLine();
            ImGui::TextUnformatted("Blend Mode");

            float opacity = selectedLayer.opacity * 100.f;
            float fill    = selectedLayer.fill    * 100.f;

            // opacity slider
            if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 100.f, "%.0f%%"))
                exec(Cmd::changeOpacity(selectedLayer.opacity, opacity * 0.01f));

            // fill slider
            if (ImGui::SliderFloat("Fill", &fill, 0.f, 100.f, "%.0f%%"))
                exec(Cmd::changeFill(selectedLayer.fill, fill * 0.01f));

            ImGui::PopID();
        }
        ImGui::Separator();

        // layer stack list ============================================================================================
        {
            const float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
            ImGui::BeginChild("LayerList", ImVec2(0, -footerHeight), ImGuiChildFlags_Borders);

            const ImGuiStyle& style   = ImGui::GetStyle();
            constexpr float rowHeight = 48.f;

            size_t moveFrom           = SIZE_MAX;
            size_t moveTo             = SIZE_MAX;

            for (size_t i = layerCount; i-- > 0; ) {
                Layer& layer = canvas[i];

                ImGui::PushID(static_cast<int32_t>(i));

                const bool   isSelected  = (state.selectedLayerID == i);
                const ImVec2 rowStartPos = ImGui::GetCursorScreenPos();

                // selectable layer row
                std::string selectID = "##row_" + std::to_string(i);
                if (ImGui::Selectable(selectID.c_str(), isSelected, ImGuiSelectableFlags_AllowOverlap, ImVec2(0, rowHeight)))
                    state.selectedLayerID = static_cast<uint32_t>(i);

                // drag and drop reordering
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                    ImGui::SetDragDropPayload("DND_LAYER_INDEX", &i, sizeof(size_t));
                    ImGui::Text("%s", layer.name.c_str());
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_LAYER_INDEX")) {
                        moveFrom = *static_cast<const size_t*>(payload->Data);
                        moveTo   = i;
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::SetCursorScreenPos(rowStartPos);

                // compute vertical centering offsets based on item heights
                constexpr float thumbOffsetY = (rowHeight - 24.f) * 0.5f;
                const     float cbOffsetY    = (rowHeight - ImGui::GetFrameHeight()) * 0.5f;

                // visibility checkbox
                ImGui::SetCursorScreenPos(ImVec2(rowStartPos.x + style.ItemSpacing.x, rowStartPos.y + cbOffsetY));
                if (ImGui::Checkbox("##visible", &layer.isActive))
                    hasChanged = true;

                // thumbnail preview
                ImGui::SetCursorScreenPos(ImVec2(ImGui::GetItemRectMax().x + style.ItemSpacing.x, rowStartPos.y + thumbOffsetY));
                constexpr ImTextureID thumbTex = static_cast<intptr_t>(0); // placeholder
                ImGui::Image(thumbTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 0.5f));

                const float  textStartX  = ImGui::GetItemRectMax().x + style.ItemSpacing.x;
                const ImVec2 textSize    = ImGui::CalcTextSize(layer.name.c_str());
                const float  textOffsetY = (rowHeight - textSize.y) * 0.5f;

                ImGui::SetCursorScreenPos(ImVec2(textStartX, rowStartPos.y + textOffsetY));
                ImGui::TextUnformatted(layer.name.c_str());

                ImGui::SetCursorScreenPos(ImVec2(rowStartPos.x, rowStartPos.y + rowHeight + style.ItemSpacing.y));
                ImGui::Dummy({ 0.f, 0.f });

                ImGui::PopID();
            }

            // reorder if a drag and drop finished
            if (moveFrom != SIZE_MAX && moveTo != SIZE_MAX && moveFrom != moveTo) {
                exec(Cmd::moveLayerToIndex(moveFrom, moveTo));
                state.selectedLayerID = static_cast<uint32_t>(moveTo); // keep moved layer selected
            }

            ImGui::EndChild();
        }
        ImGui::Separator();

        // action buttons footer =======================================================================================
        {
            if (ImGui::Button("+ New")) {
                exec(Cmd::addLayer());
                state.selectedLayerID = static_cast<uint32_t>(canvas.layerCount() - 1);
            }

            ImGui::SameLine();

            if (ImGui::Button("Duplicate"))
                exec(Cmd::duplicateLayer(state.selectedLayerID++));

            ImGui::SameLine();

            ui.disabled(layerCount <= 1, [&] {
                if (ImGui::Button("Delete"))
                    exec(Cmd::deleteLayer(state.selectedLayerID));
            });
        }

        if (hasChanged)
            ++state.version;
    });

    ui.window("History", [&] {
        // TODO
    });
}
