
add_executable(imageviewer main.cpp ImageViewer.cpp Image.cpp Texture.cpp)
target_link_libraries(imageviewer glfw glad glm)
target_include_directories(imageviewer PRIVATE ../include ../external/stb)
set_property(TARGET imageviewer PROPERTY CXX_STANDARD 17)
