# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(quadtree VERSION 1.0)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Build type
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

# generate compile commands for lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Add the executable
add_executable(
   ${PROJECT_NAME}
    main.c
    globals.c
    renderer.c
    object.c
    physics.c
    input.c
    quadtree.c
    list.c
    darray.c
)

if (${APPLE})
    message("MacOs Build")

    find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0

    target_link_libraries(${PROJECT_NAME} raylib)

    # Checks if OSX and links appropriate frameworks (only required on MacOS)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

else ()
    message("Linux Build")
    # add warning build fail
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

    # Include directories for headers
    target_include_directories(${PROJECT_NAME} PRIVATE ../${CMAKE_SOURCE_DIR})

    # Link libraries for raylib
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib GL m pthread dl rt X11)
endif ()

