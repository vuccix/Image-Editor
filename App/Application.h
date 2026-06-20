#pragma once

#include <Window/Window.h>

class Application {
public:
    Application();

    void run();

private:
    Window m_window;
};
