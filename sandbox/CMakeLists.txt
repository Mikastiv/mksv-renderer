set(APP_NAME sandbox)

set(INC_FILES
)

set(SRC_FILES
    src/main.cpp
)

add_clangformat_target(${APP_NAME} ${INC_FILES} ${SRC_FILES})

add_executable(${APP_NAME} WIN32
    ${SRC_FILES}
    ${INC_FILES}
)

add_subdirectory("shaders")

target_include_directories(${APP_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${APP_NAME}
    PRIVATE mksv_renderer
    PRIVATE D3DCompiler.lib
    PRIVATE Shlwapi.lib
)
