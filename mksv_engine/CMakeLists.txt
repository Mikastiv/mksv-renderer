set(LIB_NAME mksv_engine)

set(INC_FILES
    inc/mksv/engine.hpp
    inc/mksv/log.hpp

    inc/mksv/common/types.hpp

    inc/mksv/math/consts.hpp
    inc/mksv/math/types.hpp

    inc/mksv/string/utils.hpp

    inc/mksv/wrapper/windows.h
)

set(SRC_FILES
    src/engine.cpp
    src/log.cpp

    src/string/utils.cpp
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
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc/mksv"
)

target_link_libraries(${LIB_NAME}
    PUBLIC d3d12.lib
    PUBLIC dxgi.lib
    PUBLIC dxguid.lib
)
