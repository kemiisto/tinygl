#ifndef TINYGL_UTILS_H
#define TINYGL_UTILS_H

#include <GL/glew.h>
#include <tinygl/data_types.h>

namespace tinygl::utils {
    inline constexpr GLenum gl_enum(data_type type) {
        switch(type) {
        case data_type::gl_byte: return GL_BYTE;
        case data_type::gl_unsigned_byte: return GL_UNSIGNED_BYTE;
        case data_type::gl_short: return GL_SHORT;
        case data_type::gl_unsigned_short: return GL_UNSIGNED_SHORT;
        case data_type::gl_int: return GL_INT;
        case data_type::gl_unsigned_int: return GL_UNSIGNED_INT;
        case data_type::gl_float: return GL_FLOAT;
        case data_type::gl_double: return GL_DOUBLE;
        }
    }

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

    enum class filter {
        nearest = GL_NEAREST,
        linear = GL_LINEAR,
        nearest_mip_map_nearest = GL_NEAREST_MIPMAP_NEAREST,
        nearest_mip_map_linear = GL_NEAREST_MIPMAP_LINEAR,
        linear_mip_map_nearest = GL_LINEAR_MIPMAP_NEAREST,
        linear_mip_map_linear = GL_LINEAR_MIPMAP_LINEAR
    };
}


#endif // TINYGL_UTILS_H
