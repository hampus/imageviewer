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

#include <config.h>
#include <imageviewer/Image.h>
#include <imageviewer/glfw.h>
#include <iostream>

namespace imageviewer {

ImageViewer::ImageViewer(const std::string& image_filename) {
    // TODO: this assumes that the image fits in a single texture
    texture_ = Texture(Image(image_filename));
    shader_ = ShaderProgram(DATA_DIR "shaders/vert.glsl",
                            DATA_DIR "shaders/frag.glsl");
}

void ImageViewer::render(double time_delta) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_.use();
    texture_.bind_to_unit(GL_TEXTURE0);
    shader_.set_uniform("tex0", 0);
    shader_.set_uniform("pixel_width", pixel_width_);
    shader_.set_uniform("pixel_height", pixel_height_);
    square_.render(shader_);
}

void ImageViewer::set_size(int width, int height) {
    double imageAspect =
        static_cast<double>(texture_.get_width()) / texture_.get_height();
    double windowAspect = static_cast<double>(width) / height;
    int vx, vy, vw, vh;
    if (imageAspect > windowAspect) {
        vw = width;
        vh = static_cast<int>(width / imageAspect + 0.5);
        vx = 0;
        vy = (height - vh) / 2;
    } else {
        vw = static_cast<int>(height * imageAspect + 0.5);
        vh = height;
        vx = (width - vw) / 2;
        vy = 0;
    }
    glViewport(vx, vy, vw, vh);
    pixel_width_ = 1.0 / vw;
    pixel_height_ = 1.0 / vh;
}

} // namespace imageviewer
