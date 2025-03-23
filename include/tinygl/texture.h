#ifndef TINYGL_TEXTURE_H
#define TINYGL_TEXTURE_H

#include <filesystem>
#include <memory>
#include <string>

#include <GL/glew.h>

namespace tinygl
{
    class texture final
    {
    public:
        enum class target {
            target_1d = GL_TEXTURE_1D,
            target_2d = GL_TEXTURE_2D,
            target_3d = GL_TEXTURE_3D,
            target_1d_array = GL_TEXTURE_1D_ARRAY,
            target_2d_array = GL_TEXTURE_2D_ARRAY,
            target_rectangle = GL_TEXTURE_RECTANGLE,
            target_cube_map = GL_TEXTURE_CUBE_MAP,
            target_cube_map_array = GL_TEXTURE_CUBE_MAP_ARRAY,
            target_buffer = GL_TEXTURE_BUFFER,
            target_2d_multisample = GL_TEXTURE_2D_MULTISAMPLE,
            target_2d_multisample_array = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
        };

        enum coordinate_direction {
            s = GL_TEXTURE_WRAP_S,
            t = GL_TEXTURE_WRAP_T,
            r = GL_TEXTURE_WRAP_R
        };

        enum class wrap_mode {
            repeat = GL_REPEAT,
            mirrored_repeat = GL_MIRRORED_REPEAT,
            clamp_to_edge = GL_CLAMP_TO_EDGE,
            clamp_to_border = GL_CLAMP_TO_BORDER
        };

        enum filter {
            nearest = GL_NEAREST,
            linear = GL_LINEAR,
            nearest_mip_map_nearest = GL_NEAREST_MIPMAP_NEAREST,
            nearest_mip_map_linear = GL_NEAREST_MIPMAP_LINEAR,
            linear_mip_map_nearest = GL_LINEAR_MIPMAP_NEAREST,
            linear_mip_map_linear = GL_LINEAR_MIPMAP_LINEAR
        };

        texture(
            target target,
            const std::filesystem::path& file_name,
            GLint internal_format,
            GLenum format,
            bool gen_mip_maps,
            GLuint unit);
        ~texture();

        texture(texture&& other) noexcept;
        texture& operator=(texture&& other) noexcept;

        void bind();

        void unbind();

        void set_wrap_mode(wrap_mode mode);
        void set_wrap_mode(coordinate_direction direction, wrap_mode mode);
        wrap_mode get_wrap_mode(coordinate_direction direction) const;

        void set_minification_filter(filter filter);
        filter minification_filter() const;

        void set_magnification_filter(filter filter);
        filter magnification_filter() const;

        void set_min_mag_filters(filter minification_filter, filter magnification_filter);
        std::pair<filter, filter> min_mag_filters() const;

        static std::string to_string(const coordinate_direction& direction);
        static std::string to_string(const target& target);

    private:
        struct texture_private;
        std::unique_ptr<texture_private> p;
    };
}

#endif // TINYGL_TEXTURE_H
