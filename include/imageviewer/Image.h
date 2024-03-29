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

#ifndef IMAGEVIEWER_IMAGE_H_
#define IMAGEVIEWER_IMAGE_H_

#include <string>

namespace imageviewer {

class Image {
  public:
    Image(const std::string& filename);
    ~Image();

    // No copying
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    const unsigned char* get_data() const { return data_; }

    int get_width() const { return width_; }

    int get_height() const { return height_; }

  private:
    int width_;
    int height_;
    unsigned char* data_;
};

} // namespace imageviewer

#endif
