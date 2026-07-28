#include <View/UIManager.h>
#include "UI/UI.h"
#include "Utils.h"
#include <Model/EditorState.h>
#include <Controller/Controller.h>
#include <Controller/Commands/CanvasCommand.h>
#include <Controller/Commands/LayerCommand.h>
#include <Controller/Commands/InvertibleCommand.h>
#include <Serialization/Serialize.h>
#include <algorithm>

void UIManager::drawMenuBar(EditorState& state, Controller& controller, const std::function<void()>& onQuitRequest) {
    auto exec = [&controller, &state](std::unique_ptr<Command>&& func) {
        controller.execute(state, std::move(func));
    };

    auto openModal = [this](const std::string_view title, std::function<void()> drawContent, std::function<void()> onApply) {
        m_activePopup = ActivePopup{
            .title       = title,
            .drawContent = std::move(drawContent),
            .onApply     = std::move(onApply)
        };
    };

    auto openParamModal = [&]<typename T0>(const char* title, T0 initialVal, auto widgetFunc, auto makeCmd) {
        auto popupState = std::make_shared<T0>(initialVal);
        openModal(title,
            [popupState, widgetFunc] { widgetFunc(*popupState); },
            [popupState, exec, makeCmd] { exec(makeCmd(*popupState)); }
        );
    };

    auto openSizeModal = [&](const char* title, auto makeCmd) {
        struct SizeState { uint32_t width, height; };
        auto popupState = std::make_shared<SizeState>(state.canvas.width(), state.canvas.height());
        openModal(title,
            [popupState] {
                constexpr uint32_t min = 1;
                ImGui::DragScalar("Width",  ImGuiDataType_U32, &popupState->width,  1, &min);
                ImGui::DragScalar("Height", ImGuiDataType_U32, &popupState->height, 1, &min);
            },
            [popupState, exec, makeCmd] { exec(makeCmd(popupState->width, popupState->height)); }
        );
    };

    ui.menuBar([&] {
        ui.menu("File", [&] {
            ui.item("New...", "Ctrl+N", [&] {
                struct State { uint32_t width, height; };
                auto popupState = std::make_shared<State>(state.canvas.width(), state.canvas.height());
                openModal("New",
                    [popupState] {
                        constexpr uint32_t min = 1;
                        ImGui::DragScalar("Width",  ImGuiDataType_U32, &popupState->width,  1, &min);
                        ImGui::DragScalar("Height", ImGuiDataType_U32, &popupState->height, 1, &min);
                    },
                    [popupState, &state] { state.canvas = { popupState->width, popupState->height }; }
                );
                state.zoomLevel       = 1.f;
                state.panOffset       = { 0.f, 0.f };
                state.selectedLayerID = 0;
                state.version++;
            });
            ui.item("Open...", "Ctrl+O", [&] {
                Serialize::loadImage(state.canvas);
                state.selectedLayerID = 0;
                ++state.version;
            });
            ui.separator();
            ui.item("Save", "Ctrl+S", [&] { Serialize::saveImage(state.canvas); });
            ui.item("Save As...", "Ctrl+Shift+S", [&] {});
            ui.item("Save All...", "Ctrl+Shift+All", [&] {});
            ui.separator();
            ui.item("Close", "Ctrl+W", [&] {});
            ui.separator();
            ui.item("Quit", "Ctrl+Q", [&] { onQuitRequest(); });
        });

        ui.menu("Edit", [&] {
            ui.disabled(!controller.hasUndo(), [&] {
                ui.item("Undo", "Ctrl+Z", [&] { controller.undo(state); });
            });
            ui.disabled(!controller.hasRedo(), [&] {
                ui.item("Redo", "Ctrl+Y", [&] { controller.redo(state); });
            });
            ui.separator();

            ui.disabled(true, [] { // <---------------------------------------------------------------------------------

            ui.item("Cut", "Ctrl+C", [&] {});
            ui.item("Copy", "Ctrl+V", [&] {});
            ui.item("Paste", "Ctrl+P", [&] {});
            ui.separator();
            ui.item("Erase Selection", "Del", [&] {});
            ui.item("Fill Selection", "Backspace", [&] {});
            ui.item("Invert Selection", "Ctrl+I", [&] {});
            ui.item("Select All", "Ctrl+A", [&] {});
            ui.item("Deselect", "Ctrl+D", [&] {});

            }); // <----------------------------------------------------------------------------------------------------
        });

        ui.menu("Image", [&] {
            ui.item("Resize...",      "Ctrl+R",       [&] { openSizeModal("Resize", Cmd::scale);       });
            ui.item("Canvas Size...", "Ctrl+Shift+R", [&] { openSizeModal("Canvas Size", Cmd::resize); });

            ui.disabled(true, [] {
                ui.item("Crop to Selection", "Ctrl+Shift+X", [&] {});
            });

            ui.separator();
            ui.item("Flip Horizontally", [&] { exec(Cmd::flipCanvasHoriz()); });
            ui.item("Flip Vertically",   [&] { exec(Cmd::flipCanvasVert());  });
            ui.separator();
            ui.item("Rotate 90° Left",   [&] { exec(Cmd::rotateLeft());      });
            ui.item("Rotate 90° Right",  [&] { exec(Cmd::rotateRight());     });
            ui.item("Rotate 180°",       [&] { exec(Cmd::rotate180());       });
        });

        ui.menu("Layers", [&] {
            const uint32_t selectedID = state.selectedLayerID;

            ui.item("Add New Layer",   "Ctrl+Shift+N", [&] { exec(Cmd::addLayer());                 });
            ui.item("Delete Layer",    "Shift+Del",    [&] { exec(Cmd::deleteLayer(selectedID));    });
            ui.item("Duplicate Layer", "Ctrl+Shift+D", [&] { exec(Cmd::duplicateLayer(selectedID)); });
            ui.disabled(selectedID == 0, [&] {
                ui.item("Merge with Layer Below", "Ctrl+E", [&] { exec(Cmd::mergeWithLayerBelow(selectedID)); });
            });
            ui.item("Toggle Layer Visibility", "Ctrl+,", [&] {
                state.canvas[selectedID].toggleActive();
                state.canvas.updateComposite();
                state.version++;
            });
            ui.separator();
            ui.item("Flatten Image", "Ctrl+Shift+F", [&] { exec(Cmd::mergeAllLayers()); });
            ui.separator();
            ui.item("Flip Horizontally", [&] { exec(Cmd::flipLayerHoriz()); });
            ui.item("Flip Vertically",   [&] { exec(Cmd::flipLayerVert());  });
        });

        ui.menu("Filters", [&] {
            ui.menu("Adjust", [&] {
                ui.item("Brightness...", [&] {
                    openParamModal("Brightness Adjust", 0,
                        [](int& val) { ImGui::DragInt("Brightness", &val, 1, -255, 255); },
                        Cmd::brightness
                    );
                });
                ui.item("Contrast...",   [&] {
                    openParamModal("Contrast Adjust", 1.f,
                        [](float& val) { ImGui::DragFloat("Contrast", &val, 0.1f, 0.f, 10.f, "%.2f"); },
                        Cmd::contrast
                    );
                });
                ui.item("Saturation...", [&] {
                    openParamModal("Saturation Adjust", 1.f,
                        [](float& val) { ImGui::DragFloat("Saturation", &val, 0.1f, -10.f, 10.f, "%.2f"); },
                        Cmd::saturation
                    );
                });
            });

            ui.menu("Color", [&] {
                ui.item("Invert",       [&] { exec(Cmd::invert());      });
                ui.item("Invert Alpha", [&] { exec(Cmd::invertAlpha()); });
                ui.item("Grayscale",    [&] { exec(Cmd::grayscale());   });
                ui.item("Luminance",    [&] { exec(Cmd::luminance());   });
                ui.item("Sepia",        [&] { exec(Cmd::sepia());       });
            });

            ui.separator();

            ui.menu("Artistic", [&] {
                ui.item("Duo Tone...", [&] {
                    struct State {
                        float colorA[3] = { 1.f, 0.f, 0.5f };
                        float colorB[3] = { 0.f, 210.f / 255.f, 1.f };
                    };
                    auto popupState = std::make_shared<State>();

                    openModal("Duo Tone",
                        [popupState] {
                            ImGui::ColorEdit3("Color A", &popupState->colorA[0]);
                            ImGui::ColorEdit3("Color B", &popupState->colorB[0]);
                        },
                        [popupState, exec] {
                            for (size_t i = 0; i < 3; ++i) {
                                popupState->colorA[i] *= 255.f;
                                popupState->colorB[i] *= 255.f;
                            }
                            exec(Cmd::duoTone(popupState->colorA, popupState->colorB));
                        }
                    );
                });
                ui.disabled(true, [] {
                    ui.item("Water Color...", [&] {});
                    ui.item("Oil Painting...", [&] {});
                });
            });

            ui.menu("Blur", [&] {
                auto openBlurModal = [&](const char* name, auto makeCmd) {
                    openParamModal(name, 0,
                        [](int& amt) { ImGui::DragInt("Amount", &amt, 1, 0, 100); },
                        makeCmd
                    );
                };

                ui.item("Blur...",          [&] { openBlurModal("Mean Blur", Cmd::blur);             });
                ui.item("Gaussian Blur...", [&] { openBlurModal("Gaussian Blur", Cmd::gaussianBlur); });
                ui.item("Motion Blur...", [&] {
                    struct MotionState { int distance = 0; float angle = 0.f; };
                    openParamModal("Motion Blur", MotionState{},
                        [](MotionState& s) {
                            ImGui::DragInt("Distance", &s.distance, 1, 0, 1'000);
                            ImGui::DragFloat("Angle", &s.angle, 1.f, -360.f, 360.f, "%.0f");
                        },
                        [](const MotionState& s) { return Cmd::motionBlur(s.distance, s.angle); }
                    );
                });
            });

            ui.menu("Effects", [&] {
                ui.item("Swap Channels...", [&] {
                    struct State { int combination = 0; };
                    auto popupState = std::make_shared<State>();
                    openModal("Swap Channels",
                        [popupState] {
                            ImGui::RadioButton("Swap Red & Green Channels",   &popupState->combination, 0);
                            ImGui::RadioButton("Swap Red & Blue Channels",    &popupState->combination, 1);
                            ImGui::RadioButton("Swap Green & Blue Channels",  &popupState->combination, 3);
                            ui.separator();
                            ImGui::RadioButton("Swap Red & Alpha Channels",   &popupState->combination, 2);
                            ImGui::RadioButton("Swap Green & Alpha Channels", &popupState->combination, 4);
                            ImGui::RadioButton("Swap Blue & Alpha Channels",  &popupState->combination, 5);
                        },
                        [popupState, exec] { exec(Cmd::swapChannels(popupState->combination)); });
                });
                ui.item("Emboss",  [&] { exec(Cmd::emboss());  });
                ui.item("Outline", [&] { exec(Cmd::outline()); });
                ui.item("Sharpen...", [&] {
                    openParamModal("Sharpen", 0.f,
                        [](float& val) { ImGui::DragFloat("Amount", &val, 0.1f, 0.f, 10.f, "%.2f"); },
                        Cmd::sharpen
                    );
                });
                ui.item("Pixelate...", [&] {
                    ui.item("Pixelate...", [&] {
                        openParamModal("Pixelate", 1,
                            [](int& val) { ImGui::DragInt("Block Size", &val, 1, 1, 100); },
                            Cmd::pixelate
                        );
                    });
                });
            });

            ui.separator();

            ui.menu("Edge Detection", [&] {
                ui.item("Canny...",   [&] {
                    struct State { float lo = 20.f, hi = 50.f; };
                    auto popupState = std::make_shared<State>();

                    openModal("Canny",
                        [popupState] {
                            ImGui::TextUnformatted("Select thresholds:");
                            ImGui::DragFloat("Low",  &popupState->lo, 1.f, 0.f, popupState->hi,   "%.0f");
                            ImGui::DragFloat("High", &popupState->hi, 1.f, popupState->lo, 100.f, "%.0f");
                        },
                        [popupState, exec] { exec(Cmd::canny(popupState->lo, popupState->hi)); }
                    );
                });
                ui.item("Laplace", [&] { exec(Cmd::laplace()); });
                ui.item("Prewitt", [&] { exec(Cmd::prewitt()); });
                ui.item("Scharr",  [&] { exec(Cmd::scharr());  });
                ui.item("Sobel",   [&] { exec(Cmd::sobel());   });
            });

            ui.menu("Smart", [&] {
                ui.disabled(true, [] {
                    ui.item("Fourier Transform", [&] {});
                });
                ui.item("Normal Map...", [&] {
                    struct State { float strength = 1.f; bool invertY = false; };
                    auto popupState = std::make_shared<State>();

                    openModal("Normal Map Generator",
                        [popupState] {
                            ImGui::DragFloat("Strength", &popupState->strength, 0.1f, 0.f, 10.f, "%.2f");
                            ImGui::Checkbox("Invert Y-Axis", &popupState->invertY);
                        },
                        [popupState, exec] { exec(Cmd::normalMap(popupState->strength, popupState->invertY)); }
                    );
                });
                ui.item("Seam Carving...", [&] {
                    struct State { unsigned w, h; };
                    auto popupState = std::make_shared<State>(state.canvas.width(), state.canvas.height());

                    openModal("Seam Carving",
                        [popupState, &state] {
                            constexpr unsigned min  = 1;
                            const     unsigned maxW = state.canvas.width();
                            const     unsigned maxH = state.canvas.height();
                            ImGui::DragScalar("Width",  ImGuiDataType_U32, &popupState->w, 1, &min, &maxW);
                            ImGui::DragScalar("Height", ImGuiDataType_U32, &popupState->h, 1, &min, &maxH);
                        },
                        [popupState, exec] { exec(Cmd::seamCarving(popupState->w, popupState->h)); }
                    );
                });
            });
        });

        ui.menu("Settings", [&] {
            ui.item("Configure Paint++...", [&] {});
            ui.separator();
            ui.menu("Themes", [this] {
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

        ui.menu("Help", [] {
            ui.item("Documentation...", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/wiki");
            });

            ui.item("Report a Bug...", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/issues/new");
            });

            ui.separator();

            ui.item("About Paint++", [&] {
                Utils::openURL("https://github.com/vuccix/Image-Editor/blob/main/README.md");
            });
        });
    });
}
