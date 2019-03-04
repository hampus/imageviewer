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

#include <imageviewer/ImageViewer.h>
#include <iostream>
#include <string>

using imageviewer::ImageViewer;

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << "\n";
    exit(1);
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
        exit(1);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

    int max_texture_size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    std::cout << "Max texture size: " << max_texture_size << "\n";

    ImageViewer viewer;
    viewer.init(filename);

    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        viewer.render(time - last_time);
        last_time = time;

        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    return 0;
}
