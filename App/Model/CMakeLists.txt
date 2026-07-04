add_library(AppModel OBJECT EditorState.cpp)

target_link_libraries(AppModel
        PUBLIC
        Canvas
        glm
)

target_include_directories(AppModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
