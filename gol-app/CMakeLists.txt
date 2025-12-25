FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
        GIT_PROGRESS TRUE
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
    if(LINUX_WAYLAND)
        set(GLFW_BUILD_WAYLAND ON)
        set(GLFW_BUILD_X11 OFF)
    else()
        set(GLFW_BUILD_WAYLAND OFF)
        set(GLFW_BUILD_X11 ON)
    endif()
    # add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()


set(SOURCES
    main.cpp
    golapp.cpp)

set(HEADERS
    golapp.h)

add_executable(GOL-app ${SOURCES} ${HEADERS})

target_link_libraries(GOL-app VulkanWrapper)
target_link_libraries(GOL-app glfw)
