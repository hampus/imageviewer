
add_executable(imageviewer main.cpp ImageViewer.cpp Image.cpp Texture.cpp ShaderProgram.cpp)
target_link_libraries(imageviewer glfw glad glm)
target_include_directories(imageviewer PRIVATE ../include ../external/stb ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET imageviewer PROPERTY CXX_STANDARD 17)

configure_file(config.h.in config.h)

install(TARGETS imageviewer RUNTIME DESTINATION bin)
install(DIRECTORY shaders DESTINATION share/imageviewer)
