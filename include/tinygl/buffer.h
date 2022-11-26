#ifndef TINYGL_BUFFER_H
#define TINYGL_BUFFER_H

#include <GL/glew.h>
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

        void fill(const void* data, GLsizeiptr size);

        template<std::contiguous_iterator It>
        void fill(It first, It last)
        {
            fill(&(*first), (last - first) * sizeof(*first));
        }

    private:
        struct BufferPrivate;
        std::unique_ptr<BufferPrivate> p;
    };
}

#endif // TINYGL_BUFFER_H
