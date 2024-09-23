#ifndef TINYGL_SHADER_PROGRAM_H
#define TINYGL_SHADER_PROGRAM_H

#include "tinygl/shader.h"
#include <tinyla/mat.h>
#include <tinyla/vec.h>
#include <memory>

namespace tinygl
{
    class ShaderProgram final
    {
    public:
        ShaderProgram();
        ~ShaderProgram();

        ShaderProgram(ShaderProgram&& other) noexcept;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        void addShader(const std::shared_ptr<Shader>& shader);
        void addShaderFromSourceCode(Shader::Type type, std::string_view source);
        void addShaderFromSourceFile(Shader::Type type, const std::filesystem::path& fileName);
        void removeShader(std::shared_ptr<Shader> shader);

        int attributeLocation(std::string_view name) const;
        int uniformLocation(std::string_view name) const;

        void setUniformValue(int location, GLfloat value);
        void setUniformValue(int location, GLint value);
        void setUniformValue(int location, GLuint value);
        void setUniformValue(int location, GLfloat x, GLfloat y);
        void setUniformValue(int location, GLfloat x, GLfloat y, GLfloat z);
        void setUniformValue(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        void setUniformValue(int location, const tinyla::vec2f& v);
        void setUniformValue(int location, const tinyla::vec3f& v);
        void setUniformValue(int location, const tinyla::vec4f& v);

        void setUniformValue(int location, const tinyla::mat4f& m);
        void setUniformValue(int location, const GLfloat m[4][4]);
        void setUniformValue(int location, const GLfloat m[16]);

        void setAttributeValue(int location, GLfloat value);
        void setAttributeValue(int location, GLfloat x, GLfloat y);
        void setAttributeValue(int location, GLfloat x, GLfloat y, GLfloat z);
        void setAttributeValue(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        void setAttributeValue(int location, const tinyla::vec2f& v);
        void setAttributeValue(int location, const tinyla::vec3f& v);
        void setAttributeValue(int location, const tinyla::vec4f& v);

        template<typename T>
        void setUniformValue(std::string_view name, T value)
        {
            setUniformValue(uniformLocation(name), value);
        }

        void link();
        void use();
    private:
        struct ShaderProgramPrivate;
        std::unique_ptr<ShaderProgramPrivate> p;
    };
}

#endif // TINYGL_SHADER_PROGRAM_H
