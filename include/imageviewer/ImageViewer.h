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

class ImageViewer {
  public:
    ImageViewer(const std::string& image_filename);

    void render(double time_delta);

    void set_size(int width, int height);

    void key_event(int key, int action);

    void scroll_event(double offset);

    void mouse_button_event(int button, int action, double xpos, double ypos);

    void mouse_move_event(double xpos, double ypos);

  private:
    Texture texture_;
    ShaderProgram shader_;
    SquareVertexArray square_;
    int window_width_;
    int window_height_;
    GLfloat pixel_width_;
    GLfloat pixel_height_;
    bool mouse_down_;
    double button_lastx_;
    double button_lasty_;
    double scale_;
    double vertical_scale_;
    glm::vec2 translate_;
    bool srgb_enabled_;
};

} // namespace imageviewer

#endif
