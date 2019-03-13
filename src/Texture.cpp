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

#include <imageviewer/Texture.h>
#include <iostream>

namespace imageviewer {

Texture::Texture(const Image& image) : width_{0}, height_{0} {
    glGenTextures(1, &texture_);
    check_for_gl_error();
    std::cout << "Generated texture: " << texture_ << "\n";

    glBindTexture(GL_TEXTURE_2D, texture_);
    check_for_gl_error();

    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, image.get_width());
    glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, image.get_height());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image.get_width(),
                 image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image.get_data());
    check_for_gl_error();
    width_ = image.get_width();
    height_ = image.get_height();
    // glGenerateMipmap(GL_TEXTURE_2D);
    // check_for_gl_error();
    std::cout << "Uploaded texture: " << texture_ << "\n";
    std::cout << "Texture: " << width_ << " x " << height_ << "\n";
}

Texture::Texture(Texture&& other) {
    texture_ = other.texture_;
    width_ = other.width_;
    height_ = other.height_;
    other.texture_ = 0;
}

Texture& Texture::operator=(Texture&& other) {
    texture_ = other.texture_;
    width_ = other.width_;
    height_ = other.height_;
    other.texture_ = 0;
    return *this;
}

Texture::~Texture() {
    if (texture_ != 0) {
        std::cout << "Deleting texture: " << texture_ << "\n";
        glDeleteTextures(1, &texture_);
        check_for_gl_error();
    }
}

void Texture::bind_to_unit(GLenum texture_unit) {
    glActiveTexture(texture_unit);
    check_for_gl_error();
    glBindTexture(GL_TEXTURE_2D, texture_);
    check_for_gl_error();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    check_for_gl_error();
}

} // namespace imageviewer
