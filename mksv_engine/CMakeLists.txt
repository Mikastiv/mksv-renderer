set(LIB_NAME mksv_engine)

set(INC_FILES
    inc/mksv/engine.hpp
    inc/mksv/log.hpp
    inc/mksv/window.hpp
    inc/mksv/mksv_win.hpp

    inc/mksv/common/types.hpp

    inc/mksv/math/consts.hpp
    inc/mksv/math/types.hpp

    inc/mksv/utils/helpers.hpp
    inc/mksv/utils/string.hpp
)

set(SRC_FILES
    src/engine.cpp
    src/log.cpp
    src/window.cpp

    src/utils/helpers.cpp
    src/utils/string.cpp
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
