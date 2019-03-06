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

#include <imageviewer/ShaderProgram.h>

#include <fstream>
#include <iostream>

namespace imageviewer {

namespace {

std::string load_file(std::string filename) {
    std::ifstream in;
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    in.open(filename, std::ios::in | std::ios::binary);
    in.seekg(0, std::ios::end);
    std::string contents;
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
}

void load_shader_source(GLuint shader, std::string source) {
    const char* const pointers[]{source.c_str()};
    glShaderSource(shader, 1, pointers, 0);
    check_for_gl_error();
}

void compile_shader(GLuint shader) {
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buf[4096];
        GLsizei len;
        glGetShaderInfoLog(shader, sizeof(buf), &len, buf);
        std::cerr << "Shader info log: " << std::string(buf, len) << "\n";
        throw std::runtime_error("Failed to compile shader");
    }

    check_for_gl_error();
}

void link_program(GLuint program) {
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buf[4096];
        GLsizei len;
        glGetProgramInfoLog(program, sizeof(buf), &len, buf);
        std::cerr << "Program info log: " << std::string(buf, len) << "\n";
        throw std::runtime_error("Failed to link shader program");
    }

    check_for_gl_error();
}

} // namespace

ShaderProgram::ShaderProgram() : vert_shader_{0}, frag_shader_{0} {}

ShaderProgram::ShaderProgram(std::string vertex_file, std::string fragment_file)
    : vert_shader_{0}, frag_shader_{0} {
    std::cout << "Loading vertex shader " << vertex_file << "\n";
    vert_shader_ = glCreateShader(GL_VERTEX_SHADER);
    load_shader_source(vert_shader_, load_file(vertex_file));
    compile_shader(vert_shader_);

    std::cout << "Loading fragment shader " << fragment_file << "\n";
    frag_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader_source(frag_shader_, load_file(fragment_file));
    compile_shader(frag_shader_);

    std::cout << "Creating shader program\n";
    program_ = glCreateProgram();
    glAttachShader(program_, vert_shader_);
    glAttachShader(program_, frag_shader_);
    check_for_gl_error();

    std::cout << "Linking shader program\n";
    link_program(program_);
    std::cout << "Linked shader program\n";
}

ShaderProgram::~ShaderProgram() {
    if (program_ != 0) {
        std::cout << "Destroying shader program\n";
        glDeleteProgram(program_);
    }
    if (vert_shader_ != 0) {
        glDeleteShader(vert_shader_);
    }
    if (frag_shader_ != 0) {
        glDeleteShader(frag_shader_);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) {
    vert_shader_ = other.vert_shader_;
    frag_shader_ = other.frag_shader_;
    program_ = other.program_;
    other.vert_shader_ = 0;
    other.frag_shader_ = 0;
    other.program_ = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    vert_shader_ = other.vert_shader_;
    frag_shader_ = other.frag_shader_;
    program_ = other.program_;
    other.vert_shader_ = 0;
    other.frag_shader_ = 0;
    other.program_ = 0;
    return *this;
}

void ShaderProgram::use() {
    glUseProgram(program_);
    check_for_gl_error();
}

GLint ShaderProgram::get_input_location(const std::string& name) const {
    GLint location =
        glGetProgramResourceLocation(program_, GL_PROGRAM_INPUT, name.c_str());
    check_for_gl_error();
    if (location < 0) {
        throw std::runtime_error("No such shader attribute: " + name);
    }
    return location;
}

} // namespace imageviewer
