#include <View/UIManager.h>
#include "UI/UI.h"
#include "UI/EditorThemes.h"
#include "Utils.h"
#include <Renderer/Renderer.h>
#include <ImGui/imgui.h>
#include <array>

UIManager::UIManager(Renderer& renderer) : m_renderer(renderer) {
    ui.init();
    setTheme(2);
    Utils::loadAssets(m_assets);
}

void UIManager::render(EditorState& state, const std::function<void()>& onQuitRequest) {
    ui.beginFrame();
    ui.dockspace([&] {
        drawMenuBar(state, onQuitRequest);
    });

    drawToolbar(state);
    drawPropertiesPanel(state);
    drawCanvas(state);

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

        // handle inputs (zoom, pan, and touchpad)
        if (ImGui::IsWindowHovered()) {
            const bool isCtrlDown  = ImGui::IsKeyDown(ImGuiKey_LeftCtrl)  || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
            const bool isShiftDown = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);

            // zoom
            if (io.MouseWheel != 0.f && isCtrlDown) {
                const float oldZoom        = state.zoomLevel;
                state.zoomLevel            = std::max(0.5f, state.zoomLevel + io.MouseWheel * 0.1f);
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

        // draw checkerboard background
        constexpr float checkerSize = 16.f;
        const     auto  uvMax       = ImVec2(scaledWidth / checkerSize, scaledHeight / checkerSize);
        drawList->AddImage(static_cast<ImTextureID>(static_cast<intptr_t>(m_assets.get(Asset::Checkerboard))),
                           pMin, pMax, ImVec2(0, 0), uvMax);

        // draw canvas composite
        drawList->AddImage(static_cast<ImTextureID>(static_cast<intptr_t>(m_renderer.textureID())),
                           pMin, pMax);

        // draw canvas border
        const ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
        drawList->AddRect(pMin, pMax, borderColor, 0.f, 0, 2.f);
    });
}

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

void UIManager::drawPropertiesPanel(EditorState& state) {
    assert(state.canvas.layerCount() > 0);

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
            constexpr const char* blendModes[] = { "Normal", "Multiply", "Screen", "Overlay", "Darken", "Lighten" };
            static int32_t currentBlendMode    = 0; // placeholder state

            if (ImGui::Combo("##BlendMode", &currentBlendMode, blendModes, IM_ARRAYSIZE(blendModes))) {
                // selectedLayer.blendMode = currentBlendMode;
            }

            ImGui::SameLine();
            ImGui::TextUnformatted("Blend Mode");

            float opacity = selectedLayer.opacity * 100.f;
            float fill    = selectedLayer.fill    * 100.f;

            // opacity slider
            if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 100.f, "%.0f%%")) {
                selectedLayer.opacity = opacity * 0.01f;
                hasChanged            = true;
            }

            // fill slider
            if (ImGui::SliderFloat("Fill", &fill, 0.f, 100.f, "%.0f%%")) {
                selectedLayer.fill = fill * 0.01f;
                hasChanged         = true;
            }

            ImGui::PopID();
        }
        ImGui::Separator();

        // layer stack list ============================================================================================
        {
            const float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
            ImGui::BeginChild("LayerList", ImVec2(0, -footerHeight), ImGuiChildFlags_Borders);

            size_t moveFrom = SIZE_MAX;
            size_t moveTo   = SIZE_MAX;

            const ImGuiStyle& style   = ImGui::GetStyle();
            constexpr float rowHeight = 48.f;

            for (size_t i = layerCount; i-- > 0; ) {
                Layer& layer = canvas[i];

                ImGui::PushID(static_cast<int32_t>(i));

                const bool isSelected = (state.selectedLayerID == i);

                ImVec2 rowStartPos = ImGui::GetCursorScreenPos();

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

                // Compute vertical centering offsets based on item heights
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
                canvas.moveLayerToIndex(moveFrom, moveTo);
                state.selectedLayerID = static_cast<uint32_t>(moveTo); // keep moved layer selected
                hasChanged            = true;
            }

            ImGui::EndChild();
        }
        ImGui::Separator();

        // action buttons footer =======================================================================================
        {
            if (ImGui::Button("+ New")) {
                canvas.addLayer();
                state.selectedLayerID = static_cast<uint32_t>(canvas.layerCount() - 1);
                hasChanged = true;
            }
            ImGui::SameLine();

            if (ImGui::Button("Duplicate")) {
                canvas.duplicateLayer(state.selectedLayerID);
                state.selectedLayerID = static_cast<uint32_t>(canvas.layerCount() - 1);
                hasChanged = true;
            }
            ImGui::SameLine();

            const bool hasLayers = layerCount > 1;
            if (!hasLayers) ImGui::BeginDisabled();

            if (ImGui::Button("Delete")) {
                canvas.deleteLayer(state.selectedLayerID);
                hasChanged = true;

                if (canvas.layerCount() == 0)
                    state.selectedLayerID = 0;

                else if (state.selectedLayerID >= canvas.layerCount())
                    state.selectedLayerID = static_cast<uint32_t>(canvas.layerCount() - 1);
            }

            if (!hasLayers) ImGui::EndDisabled();

            if (hasChanged) {
                state.canvas.updateComposite();
                ++state.version;
            }
        }
    });

    ui.window("History", [&] {
        // TODO
    });
}

void UIManager::drawMenuBar(EditorState& state, const std::function<void()>& onQuitRequest) {
    ui.menuBar([&] {
        ui.menu("File", [&] {
            ui.item("New...", "Ctrl+N", [&] {});
            ui.item("Open...", "Ctrl+O", [&] {});
            ui.separator();
            ui.item("Save", "Ctrl+S", [&] {});
            ui.item("Save As...", "Ctrl+Shift+S", [&] {});
            ui.item("Save All...", "Ctrl+Shift+All", [&] {});
            ui.separator();
            ui.item("Close", "Ctrl+W", [&] {});
            ui.separator();
            ui.item("Quit", "Ctrl+Q", [&] {
                onQuitRequest();
            });
        });

        ui.menu("Edit", [&] {
            ui.item("Undo", "Ctrl+Z", [&] {});
            ui.item("Redo", "Ctrl+R", [&] {});
            ui.separator();
            ui.item("Cut", "Ctrl+C", [&] {});
            ui.item("Copy", "Ctrl+V", [&] {});
            ui.item("Paste", "Ctrl+P", [&] {});
            ui.separator();
            ui.item("Erase Selection", "Del", [&] {});
            ui.item("Fill Selection", "Backspace", [&] {});
            ui.item("Invert Selection", "Ctrl+I", [&] {});
            ui.item("Select All", "Ctrl+A", [&] {});
            ui.item("Deselect", "Ctrl+D", [&] {});
        });

        ui.menu("Image", [&] {
            ui.item("Resize...", "Ctrl+R", [&] {});
            ui.item("Canvas Size...", "Ctrl+Shift+R", [&] {});
            ui.item("Crop to Selection", "Ctrl+Shift+X", [&] {});
            ui.separator();
            ui.item("Flip Horizontally", [&] {});
            ui.item("Flip Vertically", [&] {});
            ui.separator();
            ui.item("Rotate 90° Left", [&] {});
            ui.item("Rotate 90° Right", [&] {});
            ui.item("Rotate 180°", [&] {});
        });

        ui.menu("Layers", [&] {
            ui.item("Add New Layer", "Ctrl+Shift+N", [&] {});
            ui.item("Delete Layer", "Shift+Del", [&] {});
            ui.item("Duplicate Layer", "Ctrl+Shift+D", [&] {});
            ui.item("Merge with Layer Below", "Ctrl+E", [&] {});
            ui.item("Toggle Layer Visibility", "Ctrl+,", [&] {});
            ui.separator();
            ui.item("Flatten Image", "Ctrl+Shift+F", [&] {});
            ui.separator();
            ui.item("Flip Horizontally", [&] {});
            ui.item("Flip Vertically", [&] {});
            ui.separator();
            ui.item("Rotate 90° Left", [&] {});
            ui.item("Rotate 90° Right", [&] {});
            ui.item("Rotate 180°", [&] {});
        });

        ui.menu("Filters", [&] {
            ui.menu("Adjust", [&] {
                ui.item("Brightness...", [&] {});
                ui.item("Contrast...", [&] {});
                ui.item("Saturation...", [&] {});
            });

            ui.menu("Color", [&] {
                ui.item("Invert", [&] {});
                ui.item("Invert Alpha", [&] {});
                ui.item("Grayscale", [&] {});
                ui.item("Luminance", [&] {});
                ui.item("Sepia", [&] {});
            });

            ui.separator();

            ui.menu("Artistic", [&] {
                ui.item("Duo Tone...", [&] {});
                ui.item("Water Color...", [&] {});
                ui.item("Oil Painting...", [&] {});
            });

            ui.menu("Blur", [&] {
                ui.item("Blur...", [&] {});
                ui.item("Gaussian Blur...", [&] {});
                ui.item("Motion Blur...", [&] {});
            });

            ui.menu("Effects", [&] {
                ui.item("Swap Channels...", [&] {});
                ui.item("Emboss", [&] {});
                ui.item("Outline", [&] {});
                ui.item("Sharpen...", [&] {});
                ui.item("Pixelate...", [&] {});
            });

            ui.separator();

            ui.menu("Edge Detection", [&] {
                ui.item("Canny", [&] {});
                ui.item("Laplace", [&] {});
                ui.item("Prewitt", [&] {});
                ui.item("Scharr", [&] {});
                ui.item("Sobel", [&] {});
            });

            ui.menu("Smart", [&] {
                ui.item("Fourier Transform", [&] {});
                ui.item("Normal Map", [&] {});
                ui.item("Seam Carving...", [&] {});
            });
        });

        ui.menu("Settings", [&] {
            ui.item("Configure Paint++...", [&] {});
            ui.separator();
            ui.menu("Themes", [&] {
                ui.item("Classic ImGui", (m_theme ==  0), [&] { setTheme(0);  });
                ui.item("Dark ImGui",    (m_theme ==  1), [&] { setTheme(1);  });
                ui.item("Light ImGui",   (m_theme ==  2), [&] { setTheme(2);  });
                ui.item("Classic Valve", (m_theme ==  3), [&] { setTheme(3);  });
                ui.item("Dracula",       (m_theme ==  4), [&] { setTheme(4);  });
                ui.item("Discord",       (m_theme ==  5), [&] { setTheme(5);  });
                ui.item("EnemyMouse",    (m_theme ==  6), [&] { setTheme(6);  });
                ui.item("Green",         (m_theme ==  7), [&] { setTheme(7);  });
                ui.item("Photoshop",     (m_theme ==  8), [&] { setTheme(8);  });
                ui.item("Visual Studio", (m_theme ==  9), [&] { setTheme(9);  });
                ui.item("ProgTest",      (m_theme == 10), [&] { setTheme(10); });
            });
            });
        });

        ui.menu("Help", [&] {
            ui.item("Documentation...", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/wiki");
            });

            ui.item("Report a Bug...", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/issues/new");
            });

            ui.separator();

            ui.item("About Paint++", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor");
            });
        });
    });
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
