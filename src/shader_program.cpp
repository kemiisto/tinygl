#include "tinygl/shader_program.h"
#include <fmt/format.h>
#include <algorithm>
#include <iostream>
#include <vector>

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

void tinygl::ShaderProgram::addShader(const std::shared_ptr<Shader>& shader) {
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

void tinygl::ShaderProgram::addShaderFromSourceCode(tinygl::Shader::Type type, std::string_view source)
{
    auto shader = std::make_shared<Shader>(type, source);
    addShader(shader);
}

void tinygl::ShaderProgram::addShaderFromSourceFile(tinygl::Shader::Type type, const std::filesystem::path& fileName)
{
    auto shader = std::make_shared<Shader>(type, fileName);
    addShader(shader);
}

void tinygl::ShaderProgram::removeShader(std::shared_ptr<Shader> shader)
{
    if (p->id && shader) {
        glDetachShader(p->id, shader->id());
        auto& shaders = p->shaders;
        shaders.erase(std::remove(shaders.begin(), shaders.end(), shader), shaders.end());
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

int tinygl::ShaderProgram::attributeLocation(std::string_view name) const {
    if (p->id && p->linked) {
        return glGetAttribLocation(p->id, name.data());
    } else {
        return -1;
    }
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

void tinygl::ShaderProgram::setUniformValue(int location, const tinygl::Vec2& v)
{
    glUniform2fv(location, 1, v.data());
}

void tinygl::ShaderProgram::setUniformValue(int location, const tinygl::Vec3& v)
{
    glUniform3fv(location, 1, v.data());
}

void tinygl::ShaderProgram::setUniformValue(int location, const tinygl::Vec4& v)
{
    glUniform4fv(location, 1, v.data());
}

void tinygl::ShaderProgram::setUniformValue(int location, const tinygl::Mat4& m)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, m.data());
}

void tinygl::ShaderProgram::setUniformValue(int location, const GLfloat m[4][4])
{
    glUniformMatrix4fv(location, 1, GL_FALSE, m[0]);
}

void tinygl::ShaderProgram::setUniformValue(int location, const GLfloat m[16])
{
    glUniformMatrix4fv(location, 1, GL_FALSE, m);
}

void tinygl::ShaderProgram::setAttributeValue(int location, GLfloat value)
{
    glVertexAttrib1fv(location, &value);
}

void tinygl::ShaderProgram::setAttributeValue(int location, GLfloat x, GLfloat y)
{
    GLfloat values[2] = {x, y};
    glVertexAttrib2fv(location, values);
}

void tinygl::ShaderProgram::setAttributeValue(int location, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat values[3] = {x, y, z};
    glVertexAttrib3fv(location, values);
}

void tinygl::ShaderProgram::setAttributeValue(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GLfloat values[4] = {x, y, z, w};
    glVertexAttrib4fv(location, values);
}

void tinygl::ShaderProgram::setAttributeValue(int location, const tinygl::Vec2& v)
{
    glVertexAttrib2fv(location, v.data());
}

void tinygl::ShaderProgram::setAttributeValue(int location, const tinygl::Vec3& v)
{
    glVertexAttrib3fv(location, v.data());
}

void tinygl::ShaderProgram::setAttributeValue(int location, const tinygl::Vec4& v)
{
    glVertexAttrib4fv(location, v.data());
}

int tinygl::ShaderProgram::uniformLocation(std::string_view name) const
{
    if (p->id && p->linked) {
        GLint location = glGetUniformLocation(p->id, name.data());
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
