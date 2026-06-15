#ifndef TINYGL_SHADER_H
#define TINYGL_SHADER_H

#include <filesystem>
#include <memory>
#include <string_view>

namespace tinygl
{
    class shader final
    {
    public:
        enum class type : uint32_t {
            gl_compute_shader,
            gl_vertex_shader,
            gl_tess_control_shader,
            gl_tess_evaluation_shader,
            gl_geometry_shader,
            gl_fragment_shader
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
        uint32_t id() const;

        struct shader_private;
        std::unique_ptr<shader_private> p;

        friend class shader_program;
    };
}

#endif // TINYGL_SHADER_H
