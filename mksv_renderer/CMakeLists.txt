set(LIB_NAME mksv_renderer)

set(INC_FILES
    inc/mksv/mksv_d3d12.hpp
    inc/mksv/engine.hpp
    inc/mksv/keyboard.hpp
    inc/mksv/keycodes.hpp
    inc/mksv/log.hpp
    inc/mksv/mksv_win.hpp

    inc/mksv/common/types.hpp

    inc/mksv/graphics/command_queue.hpp

    inc/mksv/math/consts.hpp
    inc/mksv/math/types.hpp

    inc/mksv/utils/helpers.hpp
    inc/mksv/utils/string.hpp

    inc/mksv/win/window.hpp
    inc/mksv/win/window_class.hpp
)

set(SRC_FILES
    src/engine.cpp
    src/keyboard.cpp
    src/log.cpp

    src/graphics/command_queue.cpp

    src/utils/helpers.cpp
    src/utils/string.cpp

    src/win/window.cpp
    src/win/window_proc.cpp
    src/win/window_class.cpp
)

set(RESOURCE_FILES
)

add_clangformat_target(${LIB_NAME} ${INC_FILES} ${SRC_FILES})

add_library(${LIB_NAME} STATIC
    ${SRC_FILES}
    ${INC_FILES}
    ${RESOURCE_FILES}
)

target_include_directories(${LIB_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

target_link_libraries(${LIB_NAME}
    PUBLIC d3d12.lib
    PUBLIC dxgi.lib
    PUBLIC dxguid.lib
)
