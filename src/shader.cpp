#include "tinygl/shader.h"
#include <fmt/format.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

static const std::map<tinygl::shader::type, std::string> shader_type_name = {
    { tinygl::shader::type::vertex,   "vertex" },
    { tinygl::shader::type::fragment, "fragment" }
};

struct tinygl::shader::shader_private
{
    explicit shader_private(type type) : shader_type(type) {}
    ~shader_private();

    void create();
    void compile();

    void compile_source_code(std::string_view source);
    void compile_source_file(const std::filesystem::path& file_name);

    GLuint id = 0;
    type shader_type;
    bool compiled = false;
};

tinygl::shader::shader_private::~shader_private()
{
    glDeleteShader(id);
}

void tinygl::shader::shader_private::create()
{
    id = glCreateShader(static_cast<GLenum>(shader_type));
    if (!id) {
        throw std::runtime_error(
            fmt::format("tinygl::shader: could not create {} shader!", shader_type_name.at(shader_type))
        );
    }
}

void tinygl::shader::shader_private::compile()
{
    if (!id || !glIsShader(id)) {
        throw std::runtime_error(
            fmt::format("tinygl::shader::shader_private::compile(): object with id {} is not a shader!", id)
        );
    }

    glCompileShader(id);

    GLint result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    compiled = (result != GL_FALSE);
    if (!compiled) {
        GLint length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string info_log(length, ' ');
        glGetShaderInfoLog(id, length, &length, &info_log[0]);
        std::cerr << "tinygl::shader::shader_private::compile(): " << info_log << std::endl;
        throw std::runtime_error(
            fmt::format("tinygl::shader: could not compile {} shader!", shader_type_name.at(shader_type))
        );
    }
}

void tinygl::shader::shader_private::compile_source_code(std::string_view source)
{
    const char* c_str = source.data();
    glShaderSource(id, 1, &c_str, nullptr);
    compile();
}

void tinygl::shader::shader_private::compile_source_file(const std::filesystem::path& file_name)
{
    std::ostringstream sstream;
    std::ifstream fstream;
    fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fstream.open(file_name);
        sstream << fstream.rdbuf();
        compile_source_code(sstream.str().c_str());
    } catch ([[maybe_unused]] const std::ifstream::failure& e) {
        throw std::runtime_error(
            fmt::format(
                "tinygl::shader::shader_private::compileSourceFile(): failed to read {}!", file_name.string()));
    }
}

tinygl::shader::shader(tinygl::shader::type type, std::string_view source) :
        p{std::make_unique<shader_private>(type)}
{
    p->create();
    p->compile_source_code(source);
}

tinygl::shader::shader(tinygl::shader::type type, const std::filesystem::path& file_name) :
        p{std::make_unique<shader_private>(type)}
{
    p->create();
    p->compile_source_file(file_name);
}

tinygl::shader::~shader() = default;

tinygl::shader::shader(tinygl::shader&& other) noexcept = default;

tinygl::shader& tinygl::shader::operator=(tinygl::shader&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

GLuint tinygl::shader::id() const
{
    return p->id;
}

tinygl::shader::type tinygl::shader::shader_type() const
{
    return p->shader_type;
}
