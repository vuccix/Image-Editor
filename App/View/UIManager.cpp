#include <View/UIManager.h>
#include "UI/UI.h"
#include "UI/EditorThemes.h"
#include "Utils.h"
#include <ImGui/imgui.h>
#include <array>

UIManager::UIManager() {
    ui.init();
}

void UIManager::render(EditorState& state) {
    ui.beginFrame();
    ui.dockspace([this, &state] { drawMenuBar(state); });
    drawToolbar(state);
    ui.endFrame();
}

void UIManager::drawToolbar(EditorState& state) {
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse
                                     | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
                                     | ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::Begin("Tools", nullptr, flags)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

        if (ImGui::BeginTable("ToolbarGrid", 1, ImGuiTableFlags_None)) {
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

            ImGui::EndTable();
        }

        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void UIManager::drawMenuBar(EditorState& state) {
    ui.menuBar([&] {
        ui.menu("File", [&] {
            ui.item("New...", "Ctrl+N", [&] {});
            ui.item("Open...", "Ctrl+O", [&] {});
            ui.separator();
            ui.item("Save", "Ctrl+S", [&] {});
            ui.item("Save As...", "Ctrl+Shift+S", [&] {});
            ui.item("Save All", "Ctrl+Shift+All", [&] {});
            ui.separator();
            ui.item("Close", "Ctrl+W", [&] {});
            ui.separator();
            ui.item("Quit", "Ctrl+Q", [&] {});
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
            ui.item("Canvas Size...", "Ctrl+R", [&] {});
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
                ui.item("Brightness", [&] {});
                ui.item("Contrast", [&] {});
                ui.item("Saturation", [&] {});
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
                ui.item("Seam Carving", [&] {});
            });
        });

        ui.menu("Settings", [&] {
            ui.item("Configure Pain++...", [&] {});
            ui.separator();
            ui.menu("Themes", [&] {
                ui.item("Classic ImGui", (m_selectedTheme ==  0), [&] { m_selectedTheme =  0; Themes::classicImGuiTheme(); });
                ui.item("Dark ImGui",    (m_selectedTheme ==  1), [&] { m_selectedTheme =  1; Themes::darkImGuiTheme();    });
                ui.item("Light ImGui",   (m_selectedTheme ==  2), [&] { m_selectedTheme =  2; Themes::lightImGuiTheme();   });
                ui.item("Classic Valve", (m_selectedTheme ==  3), [&] { m_selectedTheme =  3; Themes::classicValveTheme(); });
                ui.item("Dracula",       (m_selectedTheme ==  4), [&] { m_selectedTheme =  4; Themes::draculaTheme();      });
                ui.item("Discord",       (m_selectedTheme ==  5), [&] { m_selectedTheme =  5; Themes::discordTheme();      });
                ui.item("EnemyMouse",    (m_selectedTheme ==  6), [&] { m_selectedTheme =  6; Themes::enemyMouseTheme();   });
                ui.item("Green",         (m_selectedTheme ==  7), [&] { m_selectedTheme =  7; Themes::greenTheme();        });
                ui.item("Photoshop",     (m_selectedTheme ==  8), [&] { m_selectedTheme =  8; Themes::photoshopTheme();    });
                ui.item("Visual Studio", (m_selectedTheme ==  9), [&] { m_selectedTheme =  9; Themes::visualStudioTheme(); });
                ui.item("ProgTest",      (m_selectedTheme == 10), [&] { m_selectedTheme = 10; Themes::progtestTheme();     });
            });
        });

        ui.menu("Help", [&] {
            ui.item("Documentation...", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/wiki");
            });

            ui.separator();

            ui.item("Report a Bug...", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/issues/new");
            });

            ui.separator();

            ui.item("About Pain++", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor");
            });
        });
    });
}
