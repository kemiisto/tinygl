#include "tinygl/exceptions.h"
#include "tinygl/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <GL/glew.h>
#include <map>
#include <stdexcept>

const static std::map<tinygl::texture::target, GLenum> texture_bindings {
    { tinygl::texture::target::target_1d, GL_TEXTURE_BINDING_1D },
    { tinygl::texture::target::target_2d, GL_TEXTURE_BINDING_2D },
    { tinygl::texture::target::target_3d, GL_TEXTURE_BINDING_3D },
    { tinygl::texture::target::target_1d_array, GL_TEXTURE_BINDING_1D_ARRAY },
    { tinygl::texture::target::target_2d_array, GL_TEXTURE_BINDING_2D_ARRAY },
    { tinygl::texture::target::target_rectangle, GL_TEXTURE_BINDING_RECTANGLE },
    { tinygl::texture::target::target_cube_map, GL_TEXTURE_BINDING_CUBE_MAP },
    { tinygl::texture::target::target_cube_map_array, GL_TEXTURE_BINDING_CUBE_MAP_ARRAY },
    { tinygl::texture::target::target_buffer, GL_TEXTURE_BINDING_BUFFER },
    { tinygl::texture::target::target_2d_multisample, GL_TEXTURE_BINDING_2D_MULTISAMPLE },
    { tinygl::texture::target::target_2d_multisample_array, GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY }
};

struct tinygl::texture::texture_private
{
    explicit texture_private(target t, GLuint u);
    ~texture_private() = default;

    bool bound();

    target texture_target;
    GLuint id = 0;
    GLuint unit = 0;

    std::map<texture::coordinate_direction, texture::wrap_mode> wrap_modes = {
        { texture::coordinate_direction::s, texture::wrap_mode::repeat },
        { texture::coordinate_direction::t, texture::wrap_mode::repeat },
        { texture::coordinate_direction::r, texture::wrap_mode::repeat }
    };

    texture::filter min_filter = nearest_mip_map_linear;
    texture::filter mag_filter = linear;
};

tinygl::texture::texture_private::texture_private(target t, GLuint u) :
        texture_target{t}, unit{u}
{
    glGenTextures(1, &id);
}

bool tinygl::texture::texture_private::bound()
{
    GLint bound_id;
    glGetIntegerv(texture_bindings.at(texture_target), &bound_id);
    return id == static_cast<GLuint>(bound_id);
}

tinygl::texture::texture(tinygl::texture::target target,
                         const std::filesystem::path& file_name,
                         GLint internalformat,
                         GLenum format,
                         bool genMipMaps,
                         GLuint unit) :
        p{std::make_unique<texture_private>(target, unit)}
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    auto* data = stbi_load(file_name.string().c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture!");
    }

    bind();

    switch (target) {
        case target::target_2d:
            glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            break;
        default:
            stbi_image_free(data);
            throw std::runtime_error("[tinygl::Texture] texture target is not handled yet!");
    }

    if (genMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    unbind();
    stbi_image_free(data);
}

tinygl::texture::~texture() = default;

tinygl::texture::texture(tinygl::texture&& other) noexcept = default;

tinygl::texture& tinygl::texture::operator=(tinygl::texture&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + p->unit);
    glBindTexture(static_cast<GLenum>(p->texture_target), p->id);
}

void tinygl::texture::unbind()
{
    glActiveTexture(GL_TEXTURE0 + p->unit);
    glBindTexture(static_cast<GLenum>(p->texture_target), 0);
}

void tinygl::texture::set_wrap_mode(tinygl::texture::wrap_mode mode)
{
    assert(p->bound());

    switch (p->texture_target) {
        case target::target_1d:
        case target::target_1d_array:
        case target::target_buffer:
            p->wrap_modes.at(coordinate_direction::s) = mode;
            glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
            break;
        case target::target_2d:
        case target::target_2d_array:
        case target::target_cube_map:
        case target::target_cube_map_array:
        case target::target_2d_multisample:
        case target::target_2d_multisample_array:
        case target::target_rectangle:
            p->wrap_modes.at(coordinate_direction::s) = p->wrap_modes.at(coordinate_direction::t) = mode;
            glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
            glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
            break;
        case target::target_3d:
            p->wrap_modes.at(coordinate_direction::s) =
                p->wrap_modes.at(coordinate_direction::t) =
                    p->wrap_modes.at(coordinate_direction::r) = mode;
            glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
            glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
            glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_WRAP_R, static_cast<GLint>(mode));
            break;
    }
}

void tinygl::texture::set_wrap_mode(
        tinygl::texture::coordinate_direction direction,
        tinygl::texture::wrap_mode mode)
{
    assert(p->bound());

    switch (p->texture_target) {
        case target::target_1d:
        case target::target_1d_array:
        case target::target_buffer:
            assert(direction == coordinate_direction::s);
            p->wrap_modes.at(direction) = mode;
            glTexParameteri(static_cast<GLenum>(p->texture_target), direction, static_cast<GLint>(mode));
            break;
        case texture::target::target_2d:
        case texture::target::target_2d_array:
        case texture::target::target_cube_map:
        case texture::target::target_cube_map_array:
        case texture::target::target_2d_multisample:
        case texture::target::target_2d_multisample_array:
        case texture::target::target_rectangle:
            assert(direction == coordinate_direction::s || direction == coordinate_direction::t);
            p->wrap_modes.at(direction) = mode;
            glTexParameteri(static_cast<GLenum>(p->texture_target), direction, static_cast<GLint>(mode));
            break;
        case target::target_3d:
            p->wrap_modes.at(direction) = mode;
            glTexParameteri(static_cast<GLenum>(p->texture_target), direction, static_cast<GLint>(mode));
            break;
    }
}

tinygl::texture::wrap_mode tinygl::texture::get_wrap_mode(tinygl::texture::coordinate_direction direction) const
{
    return p->wrap_modes.at(direction);
}

void tinygl::texture::set_minification_filter(tinygl::texture::filter filter)
{
    assert(p->bound());
    glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
    p->min_filter = filter;
}

tinygl::texture::filter tinygl::texture::minification_filter() const
{
    return p->min_filter;
}

void tinygl::texture::set_magnification_filter(tinygl::texture::filter filter)
{
    assert(p->bound());
    glTexParameteri(static_cast<GLenum>(p->texture_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
    p->mag_filter = filter;
}

tinygl::texture::filter tinygl::texture::magnification_filter() const
{
    return p->mag_filter;
}

void tinygl::texture::set_min_mag_filters(
    tinygl::texture::filter minification_filter, tinygl::texture::filter magnification_filter)
{
    assert(p->bound());
    set_minification_filter(minification_filter);
    set_magnification_filter(magnification_filter);
}

std::string tinygl::texture::to_string(const tinygl::texture::coordinate_direction& direction)
{
    switch (direction) {
        case s: return "S";
        case t: return "T";
        case r: return "R";
        default: return "";
    }
}

std::string tinygl::texture::to_string(const tinygl::texture::target& target)
{
    switch (target) {
        case target::target_1d: return "1D";
        case target::target_2d: return "2D";
        case target::target_3d: return "3D";
        case target::target_1d_array: return "1DArray";
        case target::target_2d_array: return "2DArray";
        case target::target_rectangle: return "Rectangle";
        case target::target_cube_map: return "CubeMap";
        case target::target_cube_map_array: return "CubeMapArray";
        case target::target_buffer: return "Buffer";
        case target::target_2d_multisample: return "2DMultisample";
        case target::target_2d_multisample_array: return "2DMultisampleArray";
        default: return "";
    }
}
