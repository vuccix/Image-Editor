#include "Application.h"

Application::Application() : m_window(1280, 720, "Paint++"),
                             m_ui(m_renderer) {}

void Application::run() {
    while (!m_window.shouldClose()) {
        m_window.waitEvents();

        if (m_editorState.version != m_lastUploadedVersion) {
            Canvas& canvas = m_editorState.canvas;

            if (const Image& composite = canvas.getComposite();
                        composite.width != canvas.width() || composite.height != canvas.height()) {
                m_renderer.resize(canvas.width(), canvas.height());
            }

            canvas.updateComposite();
            m_renderer.upload(canvas.getComposite());
            m_lastUploadedVersion = m_editorState.version;
        }

        m_ui.render(m_editorState, m_controller, [this] { m_window.close(); });

        m_window.swapBuffers();
    }
}
