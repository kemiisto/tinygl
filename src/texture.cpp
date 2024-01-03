#include "tinygl/exceptions.h"
#include "tinygl/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <GL/glew.h>
#include <map>
#include <stdexcept>

const static std::map<tinygl::Texture::Target, GLenum> textureBindings {
    { tinygl::Texture::Target::Target1D, GL_TEXTURE_BINDING_1D },
    { tinygl::Texture::Target::Target2D, GL_TEXTURE_BINDING_2D },
    { tinygl::Texture::Target::Target3D, GL_TEXTURE_BINDING_3D },
    { tinygl::Texture::Target::Target1DArray, GL_TEXTURE_BINDING_1D_ARRAY },
    { tinygl::Texture::Target::Target2DArray, GL_TEXTURE_BINDING_2D_ARRAY },
    { tinygl::Texture::Target::TargetRectangle, GL_TEXTURE_BINDING_RECTANGLE },
    { tinygl::Texture::Target::TargetCubeMap, GL_TEXTURE_BINDING_CUBE_MAP },
    { tinygl::Texture::Target::TargetCubeMapArray, GL_TEXTURE_BINDING_CUBE_MAP_ARRAY },
    { tinygl::Texture::Target::TargetBuffer, GL_TEXTURE_BINDING_BUFFER },
    { tinygl::Texture::Target::Target2DMultisample, GL_TEXTURE_BINDING_2D_MULTISAMPLE },
    { tinygl::Texture::Target::Target2DMultisampleArray, GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY }
};

struct tinygl::Texture::TexturePrivate
{
    explicit TexturePrivate(Target t, GLuint u);
    ~TexturePrivate() = default;

    bool bound();

    Target target;
    GLuint id = 0;
    GLuint unit = 0;

    std::map<Texture::CoordinateDirection, Texture::WrapMode> wrapModes = {
        { Texture::CoordinateDirection::S, Texture::WrapMode::Repeat },
        { Texture::CoordinateDirection::T, Texture::WrapMode::Repeat },
        { Texture::CoordinateDirection::R, Texture::WrapMode::Repeat }
    };

    Texture::Filter minFilter = NearestMipMapLinear;
    Texture::Filter magFilter = Linear;
};

tinygl::Texture::TexturePrivate::TexturePrivate(Target t, GLuint u) :
        target{t}, unit{u}
{
    glGenTextures(1, &id);
}

bool tinygl::Texture::TexturePrivate::bound()
{
    GLint boundId;
    glGetIntegerv(textureBindings.at(target), &boundId);
    return id == static_cast<GLuint>(boundId);
}

tinygl::Texture::Texture(tinygl::Texture::Target target,
                         std::string_view fileName,
                         GLint internalformat,
                         GLenum format,
                         bool genMipMaps,
                         GLuint unit) :
        p{std::make_unique<TexturePrivate>(target, unit)}
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    auto* data = stbi_load(fileName.data(), &width, &height, &channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture!");
    }

    bind();

    switch (target) {
        case Target::Target2D:
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

tinygl::Texture::~Texture() = default;

tinygl::Texture::Texture(tinygl::Texture&& other) noexcept = default;

tinygl::Texture& tinygl::Texture::operator=(tinygl::Texture&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + p->unit);
    glBindTexture(static_cast<GLenum>(p->target), p->id);
}

void tinygl::Texture::unbind()
{
    glActiveTexture(GL_TEXTURE0 + p->unit);
    glBindTexture(static_cast<GLenum>(p->target), 0);
}

void tinygl::Texture::setWrapMode(tinygl::Texture::WrapMode mode)
{
    assert(p->bound());

    switch (p->target) {
        case Target::Target1D:
        case Target::Target1DArray:
        case Target::TargetBuffer:
            p->wrapModes.at(CoordinateDirection::S) = mode;
            glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
            break;
        case Target::Target2D:
        case Target::Target2DArray:
        case Target::TargetCubeMap:
        case Target::TargetCubeMapArray:
        case Target::Target2DMultisample:
        case Target::Target2DMultisampleArray:
        case Target::TargetRectangle:
            p->wrapModes.at(CoordinateDirection::S) = p->wrapModes.at(CoordinateDirection::T) = mode;
            glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
            glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
            break;
        case Target::Target3D:
            p->wrapModes.at(CoordinateDirection::S) =
                p->wrapModes.at(CoordinateDirection::T) =
                    p->wrapModes.at(CoordinateDirection::R) = mode;
            glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
            glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
            glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_WRAP_R, static_cast<GLint>(mode));
            break;
    }
}

void tinygl::Texture::setWrapMode(
        tinygl::Texture::CoordinateDirection direction,
        tinygl::Texture::WrapMode mode)
{
    assert(p->bound());

    switch (p->target) {
        case Target::Target1D:
        case Target::Target1DArray:
        case Target::TargetBuffer:
            assert(direction == CoordinateDirection::S);
            p->wrapModes.at(direction) = mode;
            glTexParameteri(static_cast<GLenum>(p->target), direction, static_cast<GLint>(mode));
            break;
        case Texture::Target::Target2D:
        case Texture::Target::Target2DArray:
        case Texture::Target::TargetCubeMap:
        case Texture::Target::TargetCubeMapArray:
        case Texture::Target::Target2DMultisample:
        case Texture::Target::Target2DMultisampleArray:
        case Texture::Target::TargetRectangle:
            assert(direction == CoordinateDirection::S || direction == CoordinateDirection::T);
            p->wrapModes.at(direction) = mode;
            glTexParameteri(static_cast<GLenum>(p->target), direction, static_cast<GLint>(mode));
            break;
        case Target::Target3D:
            p->wrapModes.at(direction) = mode;
            glTexParameteri(static_cast<GLenum>(p->target), direction, static_cast<GLint>(mode));
            break;
    }
}

tinygl::Texture::WrapMode tinygl::Texture::wrapMode(tinygl::Texture::CoordinateDirection direction) const
{
    return p->wrapModes.at(direction);
}

void tinygl::Texture::setMinificationFilter(tinygl::Texture::Filter filter)
{
    assert(p->bound());
    glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
    p->minFilter = filter;
}

tinygl::Texture::Filter tinygl::Texture::minificationFilter() const
{
    return p->minFilter;
}

void tinygl::Texture::setMagnificationFilter(tinygl::Texture::Filter filter)
{
    assert(p->bound());
    glTexParameteri(static_cast<GLenum>(p->target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
    p->magFilter = filter;
}

tinygl::Texture::Filter tinygl::Texture::magnificationFilter() const
{
    return p->magFilter;
}

void tinygl::Texture::setMinMagFilters(
    tinygl::Texture::Filter minificationFilter, tinygl::Texture::Filter magnificationFilter)
{
    assert(p->bound());
    setMinificationFilter(minificationFilter);
    setMagnificationFilter(magnificationFilter);
}

std::string tinygl::Texture::toString(const tinygl::Texture::CoordinateDirection& direction)
{
    switch (direction) {
        case S: return "S";
        case T: return "T";
        case R: return "R";
        default: return "";
    }
}

std::string tinygl::Texture::toString(const tinygl::Texture::Target& target)
{
    switch (target) {
        case Target::Target1D: return "1D";
        case Target::Target2D: return "2D";
        case Target::Target3D: return "3D";
        case Target::Target1DArray: return "1DArray";
        case Target::Target2DArray: return "2DArray";
        case Target::TargetRectangle: return "Rectangle";
        case Target::TargetCubeMap: return "CubeMap";
        case Target::TargetCubeMapArray: return "CubeMapArray";
        case Target::TargetBuffer: return "Buffer";
        case Target::Target2DMultisample: return "2DMultisample";
        case Target::Target2DMultisampleArray: return "2DMultisampleArray";
        default: return "";
    }
}
