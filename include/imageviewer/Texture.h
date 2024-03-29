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

#ifndef IMAGEVIEWER_TEXTURE_H_
#define IMAGEVIEWER_TEXTURE_H_

#include <imageviewer/glfw.h>

#include <imageviewer/Image.h>

namespace imageviewer {

class Texture {
  public:
    Texture() : texture_{0} {}
    explicit Texture(const Image& image);
    ~Texture();

    // No copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Allow moving
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);

    void bind_to_unit(GLenum texture_unit);

    int get_width() { return width_; }
    int get_height() { return height_; }

  private:
    GLuint texture_;
    int width_;
    int height_;
};

} // namespace imageviewer

#endif
