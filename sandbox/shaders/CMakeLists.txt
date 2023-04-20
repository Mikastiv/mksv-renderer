add_custom_target(Shaders)

set(VERTEX_SHADERS
    vertex_shader.hlsl
)

set(PIXEL_SHADERS
    pixel_shader.hlsl
)

set(SHADER_FILES
    ${VERTEX_SHADERS}
    ${PIXEL_SHADERS}
)

set_source_files_properties(${VERTEX_SHADERS} PROPERTIES ShaderType "vs")
set_source_files_properties(${PIXEL_SHADERS} PROPERTIES ShaderType "ps")
set_source_files_properties(${SHADER_FILES} PROPERTIES ShaderModel "6_0")

foreach(FILE ${SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shader_type ${FILE} ShaderType)
    get_source_file_property(shader_model ${FILE} ShaderModel)
    add_custom_command(TARGET Shaders
        COMMAND fxc.exe /nologo /Emain /T${shader_type}_${shader_model} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/${APP_NAME}/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/${APP_NAME}/${FILE_WE}.pdb ${FILE}
        MAIN_DEPENDENCY ${FILE}
        COMMENT "HLSL ${FILE}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endforeach(FILE)
