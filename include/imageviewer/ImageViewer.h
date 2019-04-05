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

#ifndef IMAGEVIEWER_IMAGEVIEWER_H_
#define IMAGEVIEWER_IMAGEVIEWER_H_

#include <glm/vec2.hpp>
#include <imageviewer/ShaderProgram.h>
#include <imageviewer/SquareVertexArray.h>
#include <imageviewer/Texture.h>
#include <string>

namespace imageviewer {

enum class FilterType {
    AUTO = 0,
    BOX = 1,
    TENT = 2,
    GAUSSIAN = 3,
    LANCZOS = 4
};

class ImageViewer {
  public:
    ImageViewer(const std::string& image_filename, GLFWwindow* window);

    void render(double time_delta);

    void set_size(int width, int height);

    void key_event(int key, int action);

    void scroll_event(double offset, glm::dvec2 pos);

    void mouse_button_event(int button, int action, glm::dvec2 pos);

    void mouse_move_event(glm::dvec2 pos);

  private:
    void calc_best_fit();
    void update_window_title();
    std::string get_filter_name();

    GLFWwindow* window_;
    Texture texture_;
    ShaderProgram shader_;
    SquareVertexArray square_;
    double gaussian_sigma_;
    glm::dvec2 window_size_;
    glm::dvec2 image_size_;
    glm::dvec2 mouse_last_pos_;
    bool mouse_down_;
    double scale_;
    glm::dvec2 translate_;
    bool srgb_enabled_;
    FilterType filter_type_;
    bool best_fit_;
};

} // namespace imageviewer

#endif
