#ifndef TINYGL_TEXTURE_H
#define TINYGL_TEXTURE_H

#include <filesystem>
#include <memory>
#include <string>

#include <GL/glew.h>

namespace tinygl
{
    class Texture final
    {
    public:
        enum class Target {
            Target1D = GL_TEXTURE_1D,
            Target2D = GL_TEXTURE_2D,
            Target3D = GL_TEXTURE_3D,
            Target1DArray = GL_TEXTURE_1D_ARRAY,
            Target2DArray = GL_TEXTURE_2D_ARRAY,
            TargetRectangle = GL_TEXTURE_RECTANGLE,
            TargetCubeMap = GL_TEXTURE_CUBE_MAP,
            TargetCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
            TargetBuffer = GL_TEXTURE_BUFFER,
            Target2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
            Target2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
        };

        enum CoordinateDirection {
            S = GL_TEXTURE_WRAP_S,
            T = GL_TEXTURE_WRAP_T,
            R = GL_TEXTURE_WRAP_R
        };

        enum class WrapMode {
            Repeat = GL_REPEAT,
            MirroredRepeat = GL_MIRRORED_REPEAT,
            ClampToEdge = GL_CLAMP_TO_EDGE,
            ClampToBorder = GL_CLAMP_TO_BORDER
        };

        enum Filter {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,
            NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
            NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
            LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
        };

        Texture(
            Target target,
            const std::filesystem::path& fileName,
            GLint internalformat,
            GLenum format,
            bool genMipMaps,
            GLuint unit);
        ~Texture();

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        void bind();

        void unbind();

        void setWrapMode(WrapMode mode);
        void setWrapMode(CoordinateDirection direction, WrapMode mode);
        WrapMode wrapMode(CoordinateDirection direction) const;

        void setMinificationFilter(Filter filter);
        Filter minificationFilter() const;

        void setMagnificationFilter(Filter filter);
        Filter magnificationFilter() const;

        void setMinMagFilters(Filter minificationFilter, Filter magnificationFilter);
        std::pair<Filter, Filter> minMagFilters() const;

        static std::string toString(const CoordinateDirection& direction);
        static std::string toString(const Target& target);

    private:
        struct TexturePrivate;
        std::unique_ptr<TexturePrivate> p;
    };
}

#endif // TINYGL_TEXTURE_H
