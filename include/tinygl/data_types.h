#ifndef TINYGL_DATA_TYPES_H
#define TINYGL_DATA_TYPES_H

#include <cstdint>

namespace tinygl {
    enum class data_type : std::uint32_t {
        gl_byte,
        gl_unsigned_byte,
        gl_short,
        gl_unsigned_short,
        gl_int,
        gl_unsigned_int,
        gl_float,
        gl_double
    };
}

#endif // TINYGL_DATA_TYPES_H
