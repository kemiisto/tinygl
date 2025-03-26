#include "tinygl/shader_program.h"
#include <fmt/format.h>
#include <algorithm>
#include <iostream>
#include <vector>

struct tinygl::shader_program::shader_program_private
{
    shader_program_private();
    ~shader_program_private();

    bool has_shader(shader::type type) const;

    GLuint id = 0;
    bool linked = false;
    std::vector<std::shared_ptr<shader>> shaders;
};

tinygl::shader_program::shader_program_private::shader_program_private()
{
    id = glCreateProgram();
}

tinygl::shader_program::shader_program_private::~shader_program_private()
{
    glDeleteProgram(id);
}

bool tinygl::shader_program::shader_program_private::has_shader(tinygl::shader::type type) const
{
    for (const auto& shader : shaders) {
        if (shader->shader_type() == type) {
            return true;
        }
    }
    return false;
}

tinygl::shader_program::shader_program() : p{std::make_unique<shader_program_private>()}
{
}

tinygl::shader_program::~shader_program() = default;

tinygl::shader_program::shader_program(tinygl::shader_program&& other) noexcept = default;

tinygl::shader_program& tinygl::shader_program::operator=(shader_program&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::shader_program::add_shader(const std::shared_ptr<shader>& shader) {
    if (std::find(p->shaders.begin(), p->shaders.end(), shader) != p->shaders.end()) {
        std::cout << "Shader is already added to the program!" << std::endl;
        return;
    }
    if (shader) {
        glAttachShader(p->id, shader->id());
        p->linked = false;
        p->shaders.push_back(shader);
    }
}

void tinygl::shader_program::add_shader_from_source_code(tinygl::shader::type type, std::string_view source)
{
    auto s = std::make_shared<shader>(type, source);
    add_shader(s);
}

void tinygl::shader_program::add_shader_from_source_file(tinygl::shader::type type, const std::filesystem::path& file_name)
{
    auto s = std::make_shared<shader>(type, file_name);
    add_shader(s);
}

void tinygl::shader_program::remove_shader(std::shared_ptr<shader> shader)
{
    if (p->id && shader) {
        glDetachShader(p->id, shader->id());
        auto& shaders = p->shaders;
        shaders.erase(std::remove(shaders.begin(), shaders.end(), shader), shaders.end());
    }
    p->linked = false;
}

void tinygl::shader_program::link()
{
    if (!p->id || p->shaders.empty()) {
        return;
    }

    glLinkProgram(p->id);

    GLint result = 0;
    glGetProgramiv(p->id, GL_LINK_STATUS, &result);

    p->linked = (result != GL_FALSE);
    if (!p->linked) {
        GLint info_log_length = 0;
        glGetProgramiv(p->id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, ' ');
        glGetProgramInfoLog(p->id, info_log_length, &info_log_length, &info_log[0]);
        std::cerr << info_log << std::endl;
        throw std::runtime_error("tinygl::shader_program::link(): could not link shader program!");
    }
}

void tinygl::shader_program::use() {
    if (!p->id) {
        return;
    }
    if (!p->linked) {
        link();
    }
    glUseProgram(p->id);
}

int tinygl::shader_program::attribute_location(std::string_view name) const {
    if (p->id && p->linked) {
        return glGetAttribLocation(p->id, name.data());
    } else {
        return -1;
    }
}

void tinygl::shader_program::set_uniform_value(int location, GLfloat value)
{
    glUniform1f(location, value);
}

void tinygl::shader_program::set_uniform_value(int location, GLint value)
{
    glUniform1i(location, value);
}

void tinygl::shader_program::set_uniform_value(int location, GLuint value)
{
    glUniform1ui(location, value);
}

void tinygl::shader_program::set_uniform_value(int location, GLfloat x, GLfloat y)
{
    glUniform2f(location, x, y);
}

void tinygl::shader_program::set_uniform_value(int location, GLfloat x, GLfloat y, GLfloat z)
{
    glUniform3f(location, x, y, z);
}

void tinygl::shader_program::set_uniform_value(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUniform4f(location, x, y, z, w);
}

void tinygl::shader_program::set_uniform_value(int location, const tinyla::vec2f& v)
{
    glUniform2fv(location, 1, v.data());
}

void tinygl::shader_program::set_uniform_value(int location, const tinyla::vec3f& v)
{
    glUniform3fv(location, 1, v.data());
}

void tinygl::shader_program::set_uniform_value(int location, const tinyla::vec4f& v)
{
    glUniform4fv(location, 1, v.data());
}

void tinygl::shader_program::set_uniform_value(int location, const tinyla::mat4f& m)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, m.data());
}

void tinygl::shader_program::set_uniform_value(int location, const GLfloat m[4][4])
{
    glUniformMatrix4fv(location, 1, GL_FALSE, m[0]);
}

void tinygl::shader_program::set_uniform_value(int location, const GLfloat m[16])
{
    glUniformMatrix4fv(location, 1, GL_FALSE, m);
}

void tinygl::shader_program::set_attribute_value(int location, GLfloat value)
{
    glVertexAttrib1fv(location, &value);
}

void tinygl::shader_program::set_attribute_value(int location, GLfloat x, GLfloat y)
{
    GLfloat values[2] = {x, y};
    glVertexAttrib2fv(location, values);
}

void tinygl::shader_program::set_attribute_value(int location, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat values[3] = {x, y, z};
    glVertexAttrib3fv(location, values);
}

void tinygl::shader_program::set_attribute_value(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GLfloat values[4] = {x, y, z, w};
    glVertexAttrib4fv(location, values);
}

void tinygl::shader_program::set_attribute_value(int location, const tinyla::vec2f& v)
{
    glVertexAttrib2fv(location, v.data());
}

void tinygl::shader_program::set_attribute_value(int location, const tinyla::vec3f& v)
{
    glVertexAttrib3fv(location, v.data());
}

void tinygl::shader_program::set_attribute_value(int location, const tinyla::vec4f& v)
{
    glVertexAttrib4fv(location, v.data());
}

int tinygl::shader_program::uniform_location(std::string_view name) const
{
    if (p->id && p->linked) {
        GLint location = glGetUniformLocation(p->id, name.data());
        if (location != -1) {
            return location;
        } else {
            throw std::runtime_error(
                fmt::format("tinygl::shader_program::uniform_location(): invalid uniform variable name {}!",name));
        }
    } else {
        throw std::runtime_error(
            "tinygl::shader_program::uniform_location(): program was not created or linked!"
        );
    }
}
