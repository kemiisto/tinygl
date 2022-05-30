#include "tinygl/shader_program.h"

#include <vector>
#include <iostream>

#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

struct tinygl::ShaderProgram::ShaderProgramPrivate
{
    ShaderProgramPrivate();
    ~ShaderProgramPrivate();

    bool hasShader(Shader::Type type) const;

    GLuint id = 0;
    bool linked = false;
    std::vector<std::shared_ptr<Shader>> shaders;
};

tinygl::ShaderProgram::ShaderProgramPrivate::ShaderProgramPrivate()
{
    id = glCreateProgram();
}

tinygl::ShaderProgram::ShaderProgramPrivate::~ShaderProgramPrivate()
{
    glDeleteProgram(id);
}

bool tinygl::ShaderProgram::ShaderProgramPrivate::hasShader(tinygl::Shader::Type type) const
{
    for (const auto& shader : shaders) {
        if (shader->shaderType() == type) {
            return true;
        }
    }
    return false;
}

tinygl::ShaderProgram::ShaderProgram() : p{std::make_unique<ShaderProgramPrivate>()}
{
}

tinygl::ShaderProgram::~ShaderProgram() = default;

tinygl::ShaderProgram::ShaderProgram(tinygl::ShaderProgram&& other) noexcept = default;

tinygl::ShaderProgram& tinygl::ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::ShaderProgram::addShader(std::shared_ptr<Shader> shader) {
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

void tinygl::ShaderProgram::addShaderFromSourceCode(tinygl::Shader::Type type, const char* source)
{
    auto shader = std::make_shared<Shader>(type, source);
    addShader(shader);
}

void tinygl::ShaderProgram::addShaderFromSourceFile(tinygl::Shader::Type type, const std::string& fileName)
{
    auto shader = std::make_shared<Shader>(type, fileName);
    addShader(shader);
}

void tinygl::ShaderProgram::removeShader(std::shared_ptr<Shader> shader)
{
    if (p->id && shader) {
        glDetachShader(p->id, shader->id());
        p->shaders.erase(std::remove(p->shaders.begin(), p->shaders.end(), shader), p->shaders.end());
    }
    p->linked = false;
}

void tinygl::ShaderProgram::link()
{
    if (!p->id || p->shaders.empty()) {
        return;
    }

    glLinkProgram(p->id);

    GLint result = 0;
    glGetProgramiv(p->id, GL_LINK_STATUS, &result);

    p->linked = (result != GL_FALSE);
    if (!p->linked) {
        GLint infoLogLength = 0;
        glGetProgramiv(p->id, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength, ' ');
        glGetProgramInfoLog(p->id, infoLogLength, &infoLogLength, &infoLog[0]);
        std::cerr << infoLog << std::endl;
        throw std::runtime_error("tinygl::ShaderProgram::link(): could not link shader program!");
    }
}

void tinygl::ShaderProgram::use() {
    if (!p->id) {
        return;
    }
    if (!p->linked) {
        link();
    }
    glUseProgram(p->id);
}

void tinygl::ShaderProgram::setUniformValue(int location, GLfloat value)
{
    glUniform1f(location, value);
}

void tinygl::ShaderProgram::setUniformValue(int location, GLint value)
{
    glUniform1i(location, value);
}

void tinygl::ShaderProgram::setUniformValue(int location, GLuint value)
{
    glUniform1ui(location, value);
}

void tinygl::ShaderProgram::setUniformValue(int location, GLfloat x, GLfloat y)
{
    glUniform2f(location, x, y);
}

void tinygl::ShaderProgram::setUniformValue(int location, GLfloat x, GLfloat y, GLfloat z)
{
    glUniform3f(location, x, y, z);
}

void tinygl::ShaderProgram::setUniformValue(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUniform4f(location, x, y, z, w);
}

void tinygl::ShaderProgram::setUniformValue(int location, const glm::mat4& value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

int tinygl::ShaderProgram::uniformLocation(const char* name) const
{
    if (p->id && p->linked) {
        GLint location = glGetUniformLocation(p->id, name);
        if (location != -1) {
            return location;
        } else {
            throw std::runtime_error(
                fmt::format("tinygl::ShaderProgram::uniformLocation(): invalid uniform variable name {}!",name));
        }
    } else {
        throw std::runtime_error(
            "tinygl::ShaderProgram::uniformLocation(): program was not created or linked!"
        );
    }
}
