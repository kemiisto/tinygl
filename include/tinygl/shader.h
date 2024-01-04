#ifndef TINYGL_SHADER_H
#define TINYGL_SHADER_H

#include <filesystem>
#include <memory>
#include <string_view>

#include <GL/glew.h>

namespace tinygl
{
    class Shader final
    {
    public:
        enum class Type {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
            TessellationControl = GL_TESS_CONTROL_SHADER,
            TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
            Compute = GL_COMPUTE_SHADER
        };

        Shader(Type type, std::string_view source);
        Shader(Type type, const std::filesystem::path& fileName);
        ~Shader();

        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Type shaderType() const;

    private:
        GLuint id() const;

        struct ShaderPrivate;
        std::unique_ptr<ShaderPrivate> p;

        friend class ShaderProgram;
    };
}

#endif // TINYGL_SHADER_H
