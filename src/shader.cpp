#include "tinygl/shader.h"
#include <fmt/format.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

static const std::map<tinygl::Shader::Type, std::string> shaderTypeName = {
    { tinygl::Shader::Type::Vertex,   "vertex" },
    { tinygl::Shader::Type::Fragment, "fragment" }
};

struct tinygl::Shader::ShaderPrivate
{
    explicit ShaderPrivate(Type type) : shaderType(type) {}
    ~ShaderPrivate();

    void create();
    void compile();

    void compileSourceCode(const char* source);
    void compileSourceFile(std::string_view fileName);

    GLuint id = 0;
    Type shaderType;
    bool compiled = false;
};

tinygl::Shader::ShaderPrivate::~ShaderPrivate()
{
    glDeleteShader(id);
}

void tinygl::Shader::ShaderPrivate::create()
{
    id = glCreateShader(static_cast<GLenum>(shaderType));
    if (!id) {
        throw std::runtime_error(
            fmt::format("tinygl::Shader: could not create {} shader!", shaderTypeName.at(shaderType))
        );
    }
}

void tinygl::Shader::ShaderPrivate::compile()
{
    if (!id || !glIsShader(id)) {
        throw std::runtime_error(
            fmt::format("tinygl::Shader::ShaderPrivate::compile(): object with id {} is not a shader!", id)
        );
    }

    glCompileShader(id);

    GLint result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    compiled = (result != GL_FALSE);
    if (!compiled) {
        GLint length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string infoLog(length, ' ');
        glGetShaderInfoLog(id, length, &length, &infoLog[0]);
        std::cerr << "tinygl::Shader::ShaderPrivate::compile(): " << infoLog << std::endl;
        throw std::runtime_error(
            fmt::format("tinygl::Shader: could not compile {} shader!", shaderTypeName.at(shaderType))
        );
    }
}

void tinygl::Shader::ShaderPrivate::compileSourceCode(const char* source)
{
    glShaderSource(id, 1, &source, nullptr);
    compile();
}

void tinygl::Shader::ShaderPrivate::compileSourceFile(std::string_view fileName)
{
    std::ostringstream sstream;
    std::ifstream fstream;
    fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fstream.open(fileName.data());
        sstream << fstream.rdbuf();
        compileSourceCode(sstream.str().c_str());
    } catch ([[maybe_unused]] const std::ifstream::failure& e) {
        throw std::runtime_error(
            fmt::format(
                "tinygl::Shader::ShaderPrivate::compileSourceFile(): failed to read {}!", fileName));
    }
}

tinygl::Shader::Shader(tinygl::Shader::Type type, const char* source) :
        p{std::make_unique<ShaderPrivate>(type)}
{
    p->create();
    p->compileSourceCode(source);
}

tinygl::Shader::Shader(tinygl::Shader::Type type, std::string_view fileName) :
        p{std::make_unique<ShaderPrivate>(type)}
{
    p->create();
    p->compileSourceFile(fileName);
}

tinygl::Shader::~Shader() = default;

tinygl::Shader::Shader(tinygl::Shader&& other) noexcept = default;

tinygl::Shader& tinygl::Shader::operator=(tinygl::Shader&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

GLuint tinygl::Shader::id() const
{
    return p->id;
}

tinygl::Shader::Type tinygl::Shader::shaderType() const
{
    return p->shaderType;
}
