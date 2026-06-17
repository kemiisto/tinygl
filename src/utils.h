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
}


#endif // TINYGL_UTILS_H
