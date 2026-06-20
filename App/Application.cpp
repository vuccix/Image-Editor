#include "Application.h"

Application::Application() : m_window(1280, 720, "Paint++") {}

void Application::run() {
    while (!m_window.shouldClose()) {
        m_window.waitEvents();

        m_window.swapBuffers();
    }
}
