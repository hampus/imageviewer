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

#include <imageviewer/Image.h>
#include <iostream>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace imageviewer {

Image::Image(const std::string& filename) {
    std::cout << "Loading " << filename << "...\n";
    int channels;
    data_ = stbi_load(filename.c_str(), &width_, &height_, &channels, 3);
    if (data_ == 0) {
        throw std::runtime_error("Failed to load image");
    }
    std::cout << "Loaded image. Size: " << width_ << "x" << height_ << "\n";
}

Image::~Image() {
    if (data_ != nullptr) {
        std::cout << "Freeing image...\n";
        stbi_image_free(data_);
    }
}

} // namespace imageviewer
