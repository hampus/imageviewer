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

#ifndef IMAGEVIEWER_SHADER_PROGRAM_H_
#define IMAGEVIEWER_SHADER_PROGRAM_H_

#include <imageviewer/glfw.h>

#include <glm/mat4x4.hpp>
#include <string>

namespace imageviewer {

class ShaderProgram {
  public:
    ShaderProgram();
    ShaderProgram(std::string vertex_file, std::string fragment_file);
    ~ShaderProgram();

    // No copying
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    // Moving
    ShaderProgram(ShaderProgram&& other);
    ShaderProgram& operator=(ShaderProgram&& other);

    void use();

    GLint get_input_location(const std::string& name) const;

    void set_uniform(const std::string& name, GLint value) const;

    void set_uniform(const std::string& name, GLfloat value) const;

    void set_uniform(const std::string& name, const glm::mat4& matrix) const;

    void set_uniform(const std::string& name, const glm::vec2& vector) const;

  private:
    GLuint vert_shader_;
    GLuint frag_shader_;
    GLuint program_;
};

} // namespace imageviewer

#endif
