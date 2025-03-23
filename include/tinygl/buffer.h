#ifndef TINYGL_BUFFER_H
#define TINYGL_BUFFER_H

#include <GL/glew.h>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace tinygl
{
    class buffer final
    {
    public:
        enum class type {
            vertex_buffer = GL_ARRAY_BUFFER,
            index_buffer = GL_ELEMENT_ARRAY_BUFFER
        };

        enum class usage_pattern {
            stream_draw = GL_STREAM_DRAW,
            stream_read = GL_STREAM_READ,
            stream_copy = GL_STREAM_COPY,
            static_draw = GL_STATIC_DRAW,
            static_read = GL_STATIC_READ,
            static_copy = GL_STATIC_COPY,
            dynamic_draw = GL_DYNAMIC_DRAW,
            dynamic_read = GL_DYNAMIC_READ,
            dynamic_copy = GL_DYNAMIC_COPY
        };

        buffer(type type, usage_pattern usage_pattern);
        ~buffer();

        buffer(buffer&& other) noexcept;
        buffer& operator=(buffer&& other) noexcept;

        void bind();
        void unbind();

        void create(std::size_t size, const void* data = nullptr);
        void update(std::size_t offset, std::size_t size, const void* data);

        template<std::contiguous_iterator It>
        void create(It first, It last)
        {
            create((last - first) * sizeof(*first), &(*first));
        }

        template<std::contiguous_iterator It>
        void update(std::size_t offset, It first, It last)
        {
            update(offset, (last - first) * sizeof(*first), &(*first));
        }

    private:
        struct buffer_private;
        std::unique_ptr<buffer_private> p;
    };
}

#endif // TINYGL_BUFFER_H
