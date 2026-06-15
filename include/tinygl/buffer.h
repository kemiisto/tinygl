#ifndef TINYGL_BUFFER_H
#define TINYGL_BUFFER_H

#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace tinygl
{
    class buffer final
    {
    public:
        enum class type : uint32_t {
            vertex_buffer,
            index_buffer
        };

        enum class usage_pattern : uint32_t {
            stream_draw,
            stream_read,
            stream_copy,
            static_draw,
            static_read,
            static_copy,
            dynamic_draw,
            dynamic_read,
            dynamic_copy
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
