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

namespace {

const double PI = 3.14159265358979;

double calc_gaussian_sigma() {
    // frequency response of perceptual brightness at half sampling frequency
    double gauss_target_perceptual = 0.5;
    // Apply gamma of 0.43 (close to human perception of brightness)
    double gauss_target = std::pow(gauss_target_perceptual, 1.0 / 0.43);
    // Calculate sigma based on this frequency response at 0.5 Hz
    double sigma = std::sqrt(2.0) * std::sqrt(-std::log(gauss_target)) / PI;

    std::cout << "Gaussian target frequency response at 0.5 Hz: "
              << gauss_target_perceptual << "\n";
    std::cout << "Gaussian target (gamma corrected): " << gauss_target << "\n";
    std::cout << "Gaussian sigma: " << sigma << "\n";

    return sigma;
}

} // namespace

ImageViewer::ImageViewer(const std::string& image_filename, GLFWwindow* window)
    : window_{window}, mouse_down_{false}, scale_{1.0}, translate_{0.0f},
      srgb_enabled_{true}, filter_type_{2}, best_fit_{true},
      gaussian_sigma_{calc_gaussian_sigma()} {
    // TODO: this assumes that the image fits in a single texture
    texture_ = Texture(Image(image_filename));
    image_size_ = glm::dvec2(texture_.get_width(), texture_.get_height());
    shader_ = ShaderProgram(DATA_DIR "shaders/vert.glsl",
                            DATA_DIR "shaders/frag.glsl");
    update_window_title();
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
    float gaussian_a = 1.0 / (2.0 * gaussian_sigma_ * gaussian_sigma_);

    shader_.use();
    texture_.bind_to_unit(GL_TEXTURE0);
    shader_.set_uniform("tex0", 0);
    shader_.set_uniform("transform_pos", transform_pos);
    shader_.set_uniform("image_size", image_size_);
    shader_.set_uniform("pixel_size", pixel_size);
    shader_.set_uniform("gaussian_a", gaussian_a);
    shader_.set_uniform("srgb_enabled", srgb_enabled_ ? 1 : 0);
    shader_.set_uniform("g_filter_type", filter_type_);
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
    } else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        filter_type_--;
        if (filter_type_ < 0) {
            filter_type_ = 4;
        }
        std::cout << "Filter type: " << get_filter_name() << "\n";
    } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        filter_type_++;
        if (filter_type_ > 4) {
            filter_type_ = 0;
        }
        std::cout << "Filter type: " << get_filter_name() << "\n";
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
    }
    update_window_title();
}

void ImageViewer::scroll_event(double offset, glm::dvec2 pos) {
    double change = pow(1.075, offset);
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
    update_window_title();
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
    update_window_title();
}

void ImageViewer::update_window_title() {
    std::string scale_text =
        std::to_string((int)std::round(scale_ * 100.0)) + "%";
    std::string title = "ImageViewer " + scale_text + " (" + get_filter_name();
    if (!srgb_enabled_) {
        title += "; sRGB off";
    }
    title += ")";
    glfwSetWindowTitle(window_, title.c_str());
}

std::string ImageViewer::get_filter_name() {
    switch (filter_type_) {
    case 0:
        return "Tent";
    case 1:
        return "Gaussian";
    case 2:
        return "GaussLanczos";
    case 3:
        return "Lanczos3";
    case 4:
        return "Box";
    default:
        return "Unknown";
    }
}

} // namespace imageviewer
