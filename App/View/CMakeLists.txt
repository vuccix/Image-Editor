file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

target_sources(Editor PRIVATE ${SOURCES})

target_link_libraries(Editor
        PRIVATE
        imgui
        glm
        glfw
)
