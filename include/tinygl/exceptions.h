#ifndef TINYGL_EXCEPTIONS_H
#define TINYGL_EXCEPTIONS_H

#include <tinygl/texture.h>
#include <fmt/format.h>
#include <stdexcept>

namespace tinygl
{
    class InvalidDirection : public std::runtime_error
    {
    public:
        InvalidDirection(const Texture::CoordinateDirection& direction, const Texture::Target& target) :
            std::runtime_error(
                fmt::format(
                    "[tinygl::Texture] invalid direction {} for target {}!",
                    Texture::toString(direction),
                    Texture::toString(target)
                )
            ) {}
    };
};

#endif // TINYGL_EXCEPTIONS_H
