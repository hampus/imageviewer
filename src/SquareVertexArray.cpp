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

const GLfloat VERTEX_ATTR_POS[]{-1.0f, -1.0f, -1.0f, 1.0f,
                                1.0f,  -1.0f, 1.0f,  1.0f};
const GLfloat VERTEX_ATTR_TEX[]{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};

} // namespace

SquareVertexArray::SquareVertexArray() : buffer_pos_{0}, buffer_tex_{0} {
    std::cout << "Creating vertex buffers\n";
    glGenBuffers(1, &buffer_pos_);
    glGenBuffers(1, &buffer_tex_);
    check_for_gl_error();
    std::cout << "Buffering square vertex data\n";
    glBindBuffer(GL_ARRAY_BUFFER, buffer_pos_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_ATTR_POS), VERTEX_ATTR_POS,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_tex_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_ATTR_TEX), VERTEX_ATTR_TEX,
                 GL_STATIC_DRAW);
    check_for_gl_error();
}

SquareVertexArray::~SquareVertexArray() {
    if (buffer_tex_ != 0) {
        std::cout << "Deleting vertex buffers\n";
        glDeleteBuffers(1, &buffer_pos_);
        glDeleteBuffers(1, &buffer_tex_);
    }
}

// Moving
SquareVertexArray::SquareVertexArray(SquareVertexArray&& other) {
    buffer_pos_ = other.buffer_pos_;
    buffer_tex_ = other.buffer_tex_;
    other.buffer_pos_ = 0;
    other.buffer_tex_ = 0;
}

SquareVertexArray& SquareVertexArray::operator=(SquareVertexArray&& other) {
    buffer_pos_ = other.buffer_pos_;
    buffer_tex_ = other.buffer_tex_;
    other.buffer_pos_ = 0;
    other.buffer_tex_ = 0;
    return *this;
}

void SquareVertexArray::render(const ShaderProgram& program) {
    const GLint attr_pos = program.get_input_location("in_position");
    glBindBuffer(GL_ARRAY_BUFFER, buffer_pos_);
    check_for_gl_error();
    glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    check_for_gl_error();
    glEnableVertexAttribArray(attr_pos);
    check_for_gl_error();

    const GLint attr_tex = program.get_input_location("in_texcoord");
    glBindBuffer(GL_ARRAY_BUFFER, buffer_tex_);
    check_for_gl_error();
    glVertexAttribPointer(attr_tex, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    check_for_gl_error();
    glEnableVertexAttribArray(attr_tex);
    check_for_gl_error();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    check_for_gl_error();
}

} // namespace imageviewer
