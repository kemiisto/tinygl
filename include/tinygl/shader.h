#ifndef TINYGL_SHADER_H
#define TINYGL_SHADER_H

#include <memory>
#include <string>

#include <GL/glew.h>

namespace tinygl
{
    class Shader final
    {
    public:
        enum class Type {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER
        };

        Shader(Type type, const char* source);
        Shader(Type type, const std::string& fileName);
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
