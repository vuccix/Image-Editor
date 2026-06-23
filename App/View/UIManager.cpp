#include <View/UIManager.h>
#include "UI/UI.h"
#include "UI/EditorThemes.h"
#include "Utils.h"
#include <ImGui/imgui.h>
#include <array>
#include <algorithm>

UIManager::UIManager() {
    ui.init();
    setTheme(0);
}

void UIManager::render(EditorState& state) {
    ui.beginFrame();
    ui.dockspace([this, &state] {
        drawMenuBar(state);
    });

    drawToolbar(state);
    drawPropertiesPanel(state);
    drawCanvas(state);

    ui.endFrame();
}

void UIManager::drawCanvas(EditorState& state) {
    ui.window("Canvas", [&] {

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
    ui.window("Layers", [&] {

    });

    ui.window("History", [&] {

    });
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
            ui.item("Configure Paint++...", [&] {});
            ui.separator();
            ui.menu("Themes", [&] {
                ui.item("Classic ImGui", (m_selectedTheme ==  0), [&] { setTheme(0);  });
                ui.item("Dark ImGui",    (m_selectedTheme ==  1), [&] { setTheme(1);  });
                ui.item("Light ImGui",   (m_selectedTheme ==  2), [&] { setTheme(2);  });
                ui.item("Classic Valve", (m_selectedTheme ==  3), [&] { setTheme(3);  });
                ui.item("Dracula",       (m_selectedTheme ==  4), [&] { setTheme(4);  });
                ui.item("Discord",       (m_selectedTheme ==  5), [&] { setTheme(5);  });
                ui.item("EnemyMouse",    (m_selectedTheme ==  6), [&] { setTheme(6);  });
                ui.item("Green",         (m_selectedTheme ==  7), [&] { setTheme(7);  });
                ui.item("Photoshop",     (m_selectedTheme ==  8), [&] { setTheme(8);  });
                ui.item("Visual Studio", (m_selectedTheme ==  9), [&] { setTheme(9);  });
                ui.item("ProgTest",      (m_selectedTheme == 10), [&] { setTheme(10); });
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

            ui.item("About Paint++", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor");
            });
        });
    });
}

void UIManager::setTheme(const uint8_t theme) {
    m_selectedTheme = theme;

    switch (m_selectedTheme) {
        case  0: Themes::darkImGuiTheme();    break;
        case  1: Themes::lightImGuiTheme();   break;
        case  2: Themes::classicImGuiTheme(); break;
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
