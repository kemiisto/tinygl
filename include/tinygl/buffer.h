#ifndef TINYGL_BUFFER_H
#define TINYGL_BUFFER_H

#include <GL/glew.h>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace tinygl
{
    class Buffer final
    {
    public:
        enum class Type {
            VertexBuffer = GL_ARRAY_BUFFER,
            IndexBuffer = GL_ELEMENT_ARRAY_BUFFER
        };

        enum class UsagePattern {
            StreamDraw = GL_STREAM_DRAW,
            StreamRead = GL_STREAM_READ,
            StreamCopy = GL_STREAM_COPY,
            StaticDraw = GL_STATIC_DRAW,
            StaticRead = GL_STATIC_READ,
            StaticCopy = GL_STATIC_COPY,
            DynamicDraw = GL_DYNAMIC_DRAW,
            DynamicRead = GL_DYNAMIC_READ,
            DynamicCopy = GL_DYNAMIC_COPY
        };

        Buffer(Type type, UsagePattern usagePattern);
        ~Buffer();

        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        void bind();
        void unbind();

        void create(std::size_t size, const void* data = nullptr);
        void update(std::size_t offset, std::size_t size, const void* data);

        template<std::contiguous_iterator It>
        void create(It first, It last)
        {
            create((last - first) * sizeof(*first), &(*first));
        }

    private:
        struct BufferPrivate;
        std::unique_ptr<BufferPrivate> p;
    };
}

#endif // TINYGL_BUFFER_H
