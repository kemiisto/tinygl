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
        invalid_direction(const texture::coordinate& coordinate, const texture::target& target) :
            std::runtime_error(
                fmt::format(
                    "[tinygl::texture] invalid coordinate {} for target {}!",
                    texture::to_string(coordinate),
                    texture::to_string(target)
                )
            ) {}
    };
};

#endif // TINYGL_EXCEPTIONS_H
