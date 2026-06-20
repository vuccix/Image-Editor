# OpenGL ===============================================================================================================
find_package(OpenGL REQUIRED)

# glad =================================================================================================================
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# glfw =================================================================================================================
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(GLFW)

# glm ==================================================================================================================
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_compile_definitions(glm INTERFACE GLM_ENABLE_EXPERIMENTAL)

# stb ==================================================================================================================
add_library(stb STATIC stb/stb/stb_impl.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# ImGui ================================================================================================================
file(GLOB IMGUI_SRC ImGui/*.cpp)
add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
)

target_link_libraries(imgui
        PUBLIC
        glfw
        glad
        OpenGL::GL
)

# NativeFileDialog-Extended ============================================================================================
set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_INSTALL     OFF CACHE BOOL "" FORCE)

add_subdirectory(NativeFileDialog-Extended)
