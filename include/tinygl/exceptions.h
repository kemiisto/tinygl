#ifndef TINYGL_EXCEPTIONS_H
#define TINYGL_EXCEPTIONS_H

#include <tinygl/texture.h>
#include <fmt/format.h>
#include <stdexcept>

namespace tinygl
{
    class invalid_direction : public std::runtime_error
    {
    public:
        invalid_direction(const texture::coordinate_direction& direction, const texture::target& target) :
            std::runtime_error(
                fmt::format(
                    "[tinygl::texture] invalid direction {} for target {}!",
                    texture::to_string(direction),
                    texture::to_string(target)
                )
            ) {}
    };
};

#endif // TINYGL_EXCEPTIONS_H
