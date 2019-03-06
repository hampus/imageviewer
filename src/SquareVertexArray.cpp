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

#include <imageviewer/SquareVertexArray.h>

#include <iostream>

namespace imageviewer {

namespace {

const GLfloat VERTEX_DATA[]{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
}

SquareVertexArray::SquareVertexArray() : buffer_{0} {
    std::cout << "Creating vertex buffer\n";
    glGenBuffers(1, &buffer_);
    check_for_gl_error();
    std::cout << "Buffering square vertex data\n";
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_DATA), VERTEX_DATA,
                 GL_STATIC_DRAW);
    check_for_gl_error();
}

SquareVertexArray::~SquareVertexArray() {
    if (buffer_ != 0) {
        std::cout << "Deleting vertex buffer\n";
        glDeleteBuffers(1, &buffer_);
    }
}

// Moving
SquareVertexArray::SquareVertexArray(SquareVertexArray&& other) {
    buffer_ = other.buffer_;
    other.buffer_ = 0;
}

SquareVertexArray& SquareVertexArray::operator=(SquareVertexArray&& other) {
    buffer_ = other.buffer_;
    other.buffer_ = 0;
    return *this;
}

void SquareVertexArray::render(const ShaderProgram& program) {
    const GLint position_attr = program.get_input_location("in_position");
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    check_for_gl_error();
    glVertexAttribPointer(position_attr, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    check_for_gl_error();
    glEnableVertexAttribArray(position_attr);
    check_for_gl_error();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    check_for_gl_error();
}

} // namespace imageviewer
