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

#include <imageviewer/glfw.h>

#include <imageviewer/ShaderProgram.h>

#ifndef IMAGEVIEWER_SQUARE_VERTEX_ARRAY_H_
#define IMAGEVIEWER_SQUARE_VERTEX_ARRAY_H_

namespace imageviewer {

class SquareVertexArray {
  public:
    SquareVertexArray();
    ~SquareVertexArray();

    // Moving
    SquareVertexArray(SquareVertexArray&& other);
    SquareVertexArray& operator=(SquareVertexArray&& other);

    // No copying
    SquareVertexArray(const SquareVertexArray&) = delete;
    SquareVertexArray& operator=(const SquareVertexArray&) = delete;

    void render(const ShaderProgram& program);

  private:
    GLuint buffer_pos_;
    GLuint buffer_tex_;
};

} // namespace imageviewer

#endif
