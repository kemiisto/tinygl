#ifndef TINYGL_SHADER_H
#define TINYGL_SHADER_H

#include <filesystem>
#include <memory>
#include <string_view>

#include <GL/glew.h>

namespace tinygl
{
    class shader final
    {
    public:
        enum class type {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER,
            geometry = GL_GEOMETRY_SHADER,
            tessellation_control = GL_TESS_CONTROL_SHADER,
            tessellation_evaluation = GL_TESS_EVALUATION_SHADER,
            compute = GL_COMPUTE_SHADER
        };

        shader(type type, std::string_view source);
        shader(type type, const std::filesystem::path& file_name);
        ~shader();

        shader(shader&& other) noexcept;
        shader& operator=(shader&& other) noexcept;

        shader(const shader&) = delete;
        shader& operator=(const shader&) = delete;

        type shader_type() const;

    private:
        GLuint id() const;

        struct shader_private;
        std::unique_ptr<shader_private> p;

        friend class shader_program;
    };
}

#endif // TINYGL_SHADER_H
