#ifndef TINYGL_SHADER_PROGRAM_H
#define TINYGL_SHADER_PROGRAM_H

#include "tinygl/shader.h"
#include <tinyla/mat.hpp>
#include <tinyla/vec.hpp>
#include <memory>

namespace tinygl
{
    class shader_program final
    {
    public:
        shader_program();
        ~shader_program();

        shader_program(shader_program&& other) noexcept;
        shader_program& operator=(shader_program&& other) noexcept;

        void add_shader(const std::shared_ptr<shader>& shader);
        void add_shader_from_source_code(shader::type type, std::string_view source);
        void add_shader_from_source_file(shader::type type, const std::filesystem::path& file_name);
        void remove_shader(std::shared_ptr<shader> shader);

        int attribute_location(std::string_view name) const;
        int uniform_location(std::string_view name) const;

        void set_uniform_value(int location, GLfloat value);
        void set_uniform_value(int location, GLint value);
        void set_uniform_value(int location, GLuint value);
        void set_uniform_value(int location, GLfloat x, GLfloat y);
        void set_uniform_value(int location, GLfloat x, GLfloat y, GLfloat z);
        void set_uniform_value(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        void set_uniform_value(int location, const tinyla::vec2f& v);
        void set_uniform_value(int location, const tinyla::vec3f& v);
        void set_uniform_value(int location, const tinyla::vec4f& v);

        void set_uniform_value(int location, const tinyla::mat4f& m);
        void set_uniform_value(int location, const GLfloat m[4][4]);
        void set_uniform_value(int location, const GLfloat m[16]);

        void set_attribute_value(int location, GLfloat value);
        void set_attribute_value(int location, GLfloat x, GLfloat y);
        void set_attribute_value(int location, GLfloat x, GLfloat y, GLfloat z);
        void set_attribute_value(int location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        void set_attribute_value(int location, const tinyla::vec2f& v);
        void set_attribute_value(int location, const tinyla::vec3f& v);
        void set_attribute_value(int location, const tinyla::vec4f& v);

        template<typename T>
        void set_uniform_value(std::string_view name, T value)
        {
            setUniformValue(uniform_location(name), value);
        }

        void link();
        void use();
    private:
        struct shader_program_private;
        std::unique_ptr<shader_program_private> p;
    };
}

#endif // TINYGL_SHADER_PROGRAM_H
