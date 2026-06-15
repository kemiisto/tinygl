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
        enum class binding_target : uint32_t {
            gl_array_buffer,
            gl_atomic_counter_buffer,
            gl_copy_read_buffer,
            gl_copy_write_buffer,
            gl_dispatch_indirect_buffer,
            gl_draw_indirect_buffer,
            gl_element_array_buffer,
            gl_pixel_pack_buffer,
            gl_pixel_unpack_buffer,
            gl_query_buffer,
            gl_shader_storage_buffer,
            gl_texture_buffer,
            gl_transform_feedback_buffer,
            gl_uniform_buffer
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

        buffer(binding_target binding_target, usage_pattern usage_pattern);
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
