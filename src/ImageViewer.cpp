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

#include <imageviewer/ImageViewer.h>

#include <cmath>
#include <config.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <imageviewer/Image.h>
#include <imageviewer/glfw.h>
#include <iostream>

namespace imageviewer {

ImageViewer::ImageViewer(const std::string& image_filename)
    : mouse_down_{false}, scale_{1.0}, translate_{0.0f}, srgb_enabled_{true},
      window_width_{0}, window_height_{0} {
    // TODO: this assumes that the image fits in a single texture
    texture_ = Texture(Image(image_filename));
    shader_ = ShaderProgram(DATA_DIR "shaders/vert.glsl",
                            DATA_DIR "shaders/frag.glsl");
}

void ImageViewer::render(double time_delta) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 transform(1.0f);
    transform = glm::scale(transform,
                           glm::vec3(scale_, scale_ * vertical_scale_, 1.0f));
    transform = glm::translate(transform, glm::vec3(translate_, 0.0f));

    double xscale = 1.0 / window_width_ / scale_;
    double yscale = 1.0 / window_height_ / scale_ / vertical_scale_;

    shader_.use();
    texture_.bind_to_unit(GL_TEXTURE0);
    shader_.set_uniform("tex0", 0);
    shader_.set_uniform("transform", transform);
    shader_.set_uniform("pixel_width", static_cast<float>(xscale));
    shader_.set_uniform("pixel_height", static_cast<float>(yscale));
    shader_.set_uniform("srgb_enabled", srgb_enabled_ ? 1 : 0);
    square_.render(shader_);
}

void ImageViewer::set_size(int width, int height) {
    window_width_ = width;
    window_height_ = height;
    double imageAspect =
        static_cast<double>(texture_.get_width()) / texture_.get_height();
    double windowAspect = static_cast<double>(width) / height;
    vertical_scale_ = windowAspect / imageAspect;
    glViewport(0, 0, width, height);
}

void ImageViewer::key_event(int key, int action) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        srgb_enabled_ = !srgb_enabled_;
        std::cout << "sRGB: " << srgb_enabled_ << "\n";
    }
}

void ImageViewer::scroll_event(double offset) {
    scale_ = scale_ * pow(1.1, offset);
}

void ImageViewer::mouse_button_event(int button, int action, double xpos,
                                     double ypos) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse_down_ = true;
        button_lastx_ = xpos;
        button_lasty_ = ypos;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouse_down_ = false;
    }
}

void ImageViewer::mouse_move_event(double xpos, double ypos) {
    if (mouse_down_) {
        double deltax = xpos - button_lastx_;
        double deltay = ypos - button_lasty_;
        button_lastx_ = xpos;
        button_lasty_ = ypos;
        double xscale = 2.0 / window_width_ / scale_;
        double yscale = 2.0 / window_height_ / scale_ / vertical_scale_;
        translate_ += glm::vec2(deltax, -deltay) * glm::vec2(xscale, yscale);
    }
}

} // namespace imageviewer
