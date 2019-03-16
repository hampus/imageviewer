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
#include <glm/ext/scalar_common.hpp>
#include <glm/mat4x4.hpp>
#include <imageviewer/Image.h>
#include <imageviewer/glfw.h>
#include <iostream>

namespace imageviewer {

ImageViewer::ImageViewer(const std::string& image_filename)
    : mouse_down_{false}, scale_{1.0}, translate_{0.0f}, srgb_enabled_{true},
      gaussian_enabled_{true}, best_fit_{true} {
    // TODO: this assumes that the image fits in a single texture
    texture_ = Texture(Image(image_filename));
    image_size_ = glm::dvec2(texture_.get_width(), texture_.get_height());
    shader_ = ShaderProgram(DATA_DIR "shaders/vert.glsl",
                            DATA_DIR "shaders/frag.glsl");
    gaussian_factor_ = std::sqrt(1.0 / 2.0);
}

void ImageViewer::render(double time_delta) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::dmat4 transform_pos(1.0);
    transform_pos =
        glm::scale(transform_pos, glm::dvec3(2.0 * scale_ / window_size_, 1.0));
    transform_pos = glm::translate(transform_pos, glm::dvec3(translate_, 0.0));
    transform_pos =
        glm::scale(transform_pos, glm::dvec3(image_size_ / 2.0, 1.0));

    float pixel_size = std::max(1.0 / scale_, 1.0);
    float gaussian_sigma = pixel_size * gaussian_factor_;
    std::cout << "pixel size: " << pixel_size << "\n";
    std::cout << "gaussian sigma: " << gaussian_sigma << "\n";

    shader_.use();
    texture_.bind_to_unit(GL_TEXTURE0);
    shader_.set_uniform("tex0", 0);
    shader_.set_uniform("transform_pos", transform_pos);
    shader_.set_uniform("image_size", image_size_);
    shader_.set_uniform("pixel_size", pixel_size);
    shader_.set_uniform("gaussian_sigma", gaussian_sigma);
    shader_.set_uniform("srgb_enabled", srgb_enabled_ ? 1 : 0);
    shader_.set_uniform("gaussian_enabled", gaussian_enabled_ ? 1 : 0);
    square_.render(shader_);
}

void ImageViewer::set_size(int width, int height) {
    window_size_ = glm::dvec2(width, height);

    glViewport(0, 0, width, height);

    if (best_fit_) {
        calc_best_fit();
    }
}

void ImageViewer::key_event(int key, int action) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        srgb_enabled_ = !srgb_enabled_;
        std::cout << "sRGB: " << srgb_enabled_ << "\n";
    } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        gaussian_enabled_ = !gaussian_enabled_;
        std::cout << "Gaussian: " << gaussian_enabled_ << "\n";
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        best_fit_ = true;
        std::cout << "Best fit: true\n";
        calc_best_fit();
    } else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (best_fit_) {
            best_fit_ = false;
            std::cout << "Best fit: false\n";
        }
        translate_ = glm::dvec2(0.0);
        scale_ = 1.0;
    } else if (key == GLFW_KEY_Q &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        gaussian_factor_ *= 1.1;
    } else if (key == GLFW_KEY_W &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        gaussian_factor_ /= 1.1;
    }
}

void ImageViewer::scroll_event(double offset, glm::dvec2 pos) {
    double change = pow(1.1, offset);
    double old_scale = scale_;
    scale_ = scale_ * change;
    glm::dvec2 center = window_size_ / 2.0;
    // Update translation (to keep the zooming centered on pos)
    glm::dvec2 inverted_pos = glm::dvec2(pos.x, window_size_.y - pos.y);
    translate_ = ((translate_ * old_scale + center - inverted_pos) * change +
                  inverted_pos - center) /
                 scale_;
    if (best_fit_) {
        best_fit_ = false;
        std::cout << "Best fit: false\n";
    }
}

void ImageViewer::mouse_button_event(int button, int action, glm::dvec2 pos) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse_down_ = true;
        mouse_last_pos_ = pos;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouse_down_ = false;
    }
}

void ImageViewer::mouse_move_event(glm::dvec2 pos) {
    if (mouse_down_) {
        glm::dvec2 delta = pos - mouse_last_pos_;
        mouse_last_pos_ = pos;
        translate_ += delta * glm::dvec2(1.0, -1.0) / scale_;
        if (best_fit_) {
            best_fit_ = false;
            std::cout << "Best fit: false\n";
        }
    }
}

void ImageViewer::calc_best_fit() {
    double scale0 = window_size_.x / image_size_.x;
    double scale1 = window_size_.y / image_size_.y;
    scale_ = std::min(scale0, scale1);
    translate_ = glm::dvec2(0.0);
}

} // namespace imageviewer
