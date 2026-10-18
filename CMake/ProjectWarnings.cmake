add_library(ProjectWarnings INTERFACE)

target_compile_options(ProjectWarnings
        INTERFACE
        "$<$<CXX_COMPILER_ID:MSVC>:/W4>"
        "$<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-Wpedantic>"
        "$<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic>"
        "$<$<CXX_COMPILER_ID:AppleClang>:-Wall;-Wextra;-Wpedantic>"
)
