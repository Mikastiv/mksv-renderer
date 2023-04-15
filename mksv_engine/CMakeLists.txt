set(LIB_NAME MksvEngine)

set(INC_FILES

    resource.h
)

set(SRC_FILES
)

set(RESOURCE_FILES
    DirectX12.rc
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
