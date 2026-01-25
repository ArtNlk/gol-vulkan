set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SHADER_BINARY_DIR "${CMAKE_BINARY_DIR}/gol-app/shaders")

file(MAKE_DIRECTORY "${SHADER_BINARY_DIR}")

file(GLOB_RECURSE SHADERS
    "${SHADER_SOURCE_DIR}/*.vert"
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.comp"
)

if(NOT SHADERS)
    message(STATUS "No shader files found in ${SHADER_SOURCE_DIR}")
endif()

foreach(SHADER_FILE ${SHADERS})
    file(RELATIVE_PATH REL_PATH "${SHADER_SOURCE_DIR}" "${SHADER_FILE}")

    set(OUTPUT_FILE "${SHADER_BINARY_DIR}/${REL_PATH}.spv")

    get_filename_component(OUTPUT_DIR "${OUTPUT_FILE}" DIRECTORY)
    add_custom_command(
        OUTPUT  "${OUTPUT_FILE}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
        COMMAND glslc -O
            "${SHADER_FILE}"
            -o "${OUTPUT_FILE}"
        DEPENDS "${SHADER_FILE}"
        COMMENT "Compiling shader: ${REL_PATH}"
        VERBATIM
    )

    # Collect all outputs
    list(APPEND ALL_SPV_FILES "${OUTPUT_FILE}")
endforeach()

add_custom_target(compile_shaders ALL
    DEPENDS ${ALL_SPV_FILES}
    SOURCES ${SHADERS}
    COMMENT "Compiling all shaders..."
)

# Optional: give the target a folder in IDEs
set_target_properties(compile_shaders PROPERTIES FOLDER "Shaders")
