#ifndef TINYGL_SHADER_PROGRAM_H
#define TINYGL_SHADER_PROGRAM_H

#include "tinygl/shader.h"
#include "tinygl/vec.h"
#include "tinygl/mat.h"
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
        void addShaderFromSourceCode(Shader::Type type, const char* source);
        void addShaderFromSourceFile(Shader::Type type, const std::string& fileName);
        void removeShader(std::shared_ptr<Shader> shader);

        int attributeLocation(const char* name) const;
        int uniformLocation(const char* name) const;

        void setUniformValue(int location, GLfloat value);
        void setUniformValue(int location, GLint value);
        void setUniformValue(int location, GLuint value);
        void setUniformValue(int location, GLfloat x, GLfloat y);
        void setUniformValue(int location, GLfloat x, GLfloat y, GLfloat z);
        void setUniformValue(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        void setUniformValue(int location, const tinygl::Vec3& value);
        void setUniformValue(int location, const tinygl::Mat4& value);

        template<typename T>
        void setUniformValue(const char* name, T value)
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
