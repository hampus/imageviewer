/**
 * Copyright 2019 Hampus Wessman
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <imageviewer/glfw.h>

#include <glm/vec2.hpp>
#include <imageviewer/ImageViewer.h>
#include <iostream>
#include <string>

using imageviewer::ImageViewer;

namespace {

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << "\n";
    glfwTerminate();
    exit(1);
}

void GLAPIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar* message,
                                    const void* userParam) {
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    auto viewer = static_cast<ImageViewer*>(glfwGetWindowUserPointer(window));
    viewer->set_size(width, height);
}

void init_window_size(ImageViewer& viewer, GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    viewer.set_size(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods) {
    auto viewer = static_cast<ImageViewer*>(glfwGetWindowUserPointer(window));
    viewer->key_event(key, action);
}

glm::dvec2 get_mouse_pos(GLFWwindow* window) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::dvec2(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto viewer = static_cast<ImageViewer*>(glfwGetWindowUserPointer(window));
    viewer->scroll_event(yoffset, get_mouse_pos(window));
}

void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods) {
    auto viewer = static_cast<ImageViewer*>(glfwGetWindowUserPointer(window));
    viewer->mouse_button_event(button, action, get_mouse_pos(window));
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto viewer = static_cast<ImageViewer*>(glfwGetWindowUserPointer(window));
    viewer->mouse_move_event(glm::dvec2(xpos, ypos));
}

} // namespace

void main_loop(const std::string& filename, GLFWwindow* window) {
    ImageViewer viewer(filename);

    glfwSetWindowUserPointer(window, &viewer);
    glfwSetWindowSizeCallback(window, window_size_callback);
    init_window_size(viewer, window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        viewer.render(time - last_time);
        last_time = time;

        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Starting image viewer...\n";

    if (argc != 2) {
        std::cerr << "Usage: imageviewer {image file}\n";
        exit(2);
    }
    const std::string filename{argv[1]};

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        exit(1);
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Image viewer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create a window\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_message_callback, 0);

    int max_texture_size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    std::cout << "Max texture size: " << max_texture_size << "\n";

    main_loop(filename, window);

    std::cout << "Shutting down\n";
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
