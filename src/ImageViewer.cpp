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

void ImageViewer::init(const std::string& image_filename) {
    // TODO: this assumes that the image fits in a single texture
    texture_ = Texture(Image(image_filename));
    shader_ = ShaderProgram(DATA_DIR "shaders/vert.glsl",
                            DATA_DIR "shaders/frag.glsl");
}

void ImageViewer::render(double time_delta) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace imageviewer
