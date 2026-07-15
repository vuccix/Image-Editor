#include <View/UIManager.h>
#include "UI/UI.h"
#include "Utils.h"
#include <Controller/Controller.h>
#include <Controller/Commands/CanvasCommand.h>
#include <Controller/Commands/LayerCommand.h>
#include <algorithm>

void UIManager::drawMenuBar(EditorState& state, Controller& controller, const std::function<void()>& onQuitRequest) {
    auto exec = [&controller, &state](std::unique_ptr<Command>&& func) {
        controller.execute(state, std::move(func));
    };

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
            ui.disabled(!controller.hasUndo(), [&] {
                ui.item("Undo", "Ctrl+Z", [&] { controller.undo(state); });
            });
            ui.disabled(!controller.hasRedo(), [&] {
                ui.item("Redo", "Ctrl+R", [&] { controller.redo(state); });
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
            ui.item("Resize...",      "Ctrl+R",       [&] { exec(Cmd::scale(800, 1800));   });
            ui.item("Canvas Size...", "Ctrl+Shift+R", [&] { exec(Cmd::resize(1500, 1000)); });

            ui.disabled(true, [] {
                ui.item("Crop to Selection", "Ctrl+Shift+X", [&] {});
            });

            ui.separator();
            ui.item("Flip Horizontally", [&] { exec(Cmd::flipHorizCanvas()); });
            ui.item("Flip Vertically",   [&] { exec(Cmd::flipVertCanvas());  });
            ui.separator();
            ui.item("Rotate 90° Left",  [&] { exec(Cmd::rotateLeft());  });
            ui.item("Rotate 90° Right", [&] { exec(Cmd::rotateRight()); });
            ui.item("Rotate 180°",      [&] { exec(Cmd::rotate180());   });
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
            ui.item("Flip Horizontally", [&] { exec(Cmd::flipHoriz()); });
            ui.item("Flip Vertically",   [&] { exec(Cmd::flipVert());  });
        });

        ui.menu("Filters", [&] {
            ui.menu("Adjust", [&] {
                ui.item("Brightness...", [&] { exec(Cmd::brightness(25));   });
                ui.item("Contrast...",   [&] { exec(Cmd::contrast(5.f));    });
                ui.item("Saturation...", [&] { exec(Cmd::saturation(-5.f)); });
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
                    constexpr float colorA[3] = { 255.f,   0.f, 127.f };
                    constexpr float colorB[3] = {   0.f, 210.f, 255.f };
                    exec(Cmd::duoTone(colorA, colorB));
                });
                ui.item("Water Color...", [&] {});
                ui.item("Oil Painting...", [&] {});
            });

            ui.menu("Blur", [&] {
                ui.item("Blur...", [&] { exec(Cmd::blur(1)); });
                ui.item("Gaussian Blur...", [&] { exec(Cmd::gaussianBlur(1)); });
                ui.item("Motion Blur...", [&] { exec(Cmd::motionBlur(150, -25.f)); });
            });

            ui.menu("Effects", [&] {
                ui.item("Swap Channels...", [&] { exec(Cmd::swapChannels(1)); });
                ui.item("Emboss",  [&] { exec(Cmd::emboss());  });
                ui.item("Outline", [&] { exec(Cmd::outline()); });
                ui.item("Sharpen...", [&] { exec(Cmd::sharpen(100.f)); });
                ui.item("Pixelate...", [&] { exec(Cmd::pixelate(13)); });
            });

            ui.separator();

            ui.menu("Edge Detection", [&] {
                ui.item("Canny",   [&] { exec(Cmd::canny(20.f, 50.f)); });
                ui.item("Laplace", [&] { exec(Cmd::laplace()); });
                ui.item("Prewitt", [&] { exec(Cmd::prewitt()); });
                ui.item("Scharr",  [&] { exec(Cmd::scharr());  });
                ui.item("Sobel",   [&] { exec(Cmd::sobel());   });
            });

            ui.menu("Smart", [&] {
                ui.item("Fourier Transform", [&] {});
                ui.item("Normal Map", [&] { exec(Cmd::normalMap(1.f, false)); });
                ui.item("Seam Carving...", [&] { exec(Cmd::seamCarving(1'000, 900)); });
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

        ui.menu("Help", [&] {
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
