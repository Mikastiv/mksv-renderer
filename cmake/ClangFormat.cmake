function(add_clangformat_target target)
    set(CLANGFORMAT_EXEC clang-format)

    foreach(src_file ${ARGN})
        get_filename_component(src_file ${src_file} ABSOLUTE)
        list(APPEND SRC_${target} ${src_file})
    endforeach()

    add_custom_target(Format_${target}
        COMMAND ${CLANGFORMAT_EXEC}
        -style=file
        -i
        ${SRC_${target}}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endfunction()
