#include <Window/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace {

void glfwErrorCallback(const int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << '\n';
}

} // namespace

Window::Window(const int width, const int height, const char* title) : m_windowTitle(title) {
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // OpenGL 3.3 Core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMaximizeWindow(m_window);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(m_window, this);

    // load OpenGL function pointers via GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("Failed to initialize GLAD");
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::waitEvents() const {
    glfwWaitEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(m_window);
}

void Window::clear(const float r, const float g, const float b, const float a) const {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    glViewport(0, 0, w, h);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Window::setTitle(const char* title) const {
    glfwSetWindowTitle(m_window, title);
}

void Window::close() const {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

std::string Window::getTitle()    const { return m_windowTitle; }
bool        Window::isFocused()   const { return m_isFocused;   }
bool        Window::isMinimized() const { return m_isMinimized; }
GLFWwindow* Window::getNative()   const { return m_window;      }
