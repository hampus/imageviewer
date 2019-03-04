cmake_minimum_required(VERSION 3.5)
project(imageviewer LANGUAGES C CXX)

set(DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/imageviewer/)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# Glad
add_subdirectory(external/glad)

# GLM
add_subdirectory(external/glm)

# Build application
add_subdirectory(src)
