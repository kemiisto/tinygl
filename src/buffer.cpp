#include "tinygl/buffer.h"
#include <GL/glew.h>

namespace {
    constexpr GLenum gl_enum(tinygl::buffer::binding_target target)
    {
        switch(target) {
        case tinygl::buffer::binding_target::gl_array_buffer: return GL_ARRAY_BUFFER;
        case tinygl::buffer::binding_target::gl_atomic_counter_buffer: return GL_ATOMIC_COUNTER_BUFFER;
        case tinygl::buffer::binding_target::gl_copy_read_buffer: return GL_COPY_READ_BUFFER;
        case tinygl::buffer::binding_target::gl_copy_write_buffer: return GL_COPY_WRITE_BUFFER;
        case tinygl::buffer::binding_target::gl_dispatch_indirect_buffer: return GL_DISPATCH_INDIRECT_BUFFER;
        case tinygl::buffer::binding_target::gl_draw_indirect_buffer: return GL_DRAW_INDIRECT_BUFFER;
        case tinygl::buffer::binding_target::gl_element_array_buffer: return GL_ELEMENT_ARRAY_BUFFER;
        case tinygl::buffer::binding_target::gl_pixel_pack_buffer: return GL_PIXEL_PACK_BUFFER;
        case tinygl::buffer::binding_target::gl_pixel_unpack_buffer: return GL_PIXEL_UNPACK_BUFFER;
        case tinygl::buffer::binding_target::gl_query_buffer: return GL_QUERY_BUFFER;
        case tinygl::buffer::binding_target::gl_shader_storage_buffer: return GL_SHADER_STORAGE_BUFFER;
        case tinygl::buffer::binding_target::gl_texture_buffer: return GL_TEXTURE_BUFFER;
        case tinygl::buffer::binding_target::gl_transform_feedback_buffer: return GL_TRANSFORM_FEEDBACK_BUFFER;
        case tinygl::buffer::binding_target::gl_uniform_buffer: return GL_UNIFORM_BUFFER;
        }
    }

    constexpr GLenum gl_enum(tinygl::buffer::usage_pattern usage_pattern)
    {
        switch(usage_pattern) {
        case tinygl::buffer::usage_pattern::gl_stream_draw: return GL_STREAM_DRAW;
        case tinygl::buffer::usage_pattern::gl_stream_read: return GL_STREAM_READ;
        case tinygl::buffer::usage_pattern::gl_stream_copy: return GL_STREAM_COPY;
        case tinygl::buffer::usage_pattern::gl_static_draw: return GL_STATIC_DRAW;
        case tinygl::buffer::usage_pattern::gl_static_read: return GL_STATIC_READ;
        case tinygl::buffer::usage_pattern::gl_static_copy: return GL_STATIC_COPY;
        case tinygl::buffer::usage_pattern::gl_dynamic_draw: return GL_DYNAMIC_DRAW;
        case tinygl::buffer::usage_pattern::gl_dynamic_read: return GL_DYNAMIC_READ;
        case tinygl::buffer::usage_pattern::gl_dynamic_copy: return GL_DYNAMIC_COPY;
        }
    }
}

struct tinygl::buffer::buffer_private
{
    buffer_private(buffer::binding_target binding_target, buffer::usage_pattern pattern);
    ~buffer_private();

    GLuint id = 0;
    binding_target binding_target;
    usage_pattern usage_pattern;
};

tinygl::buffer::buffer_private::buffer_private(tinygl::buffer::binding_target target, buffer::usage_pattern pattern) :
        binding_target{target},
        usage_pattern{pattern}
{
    glGenBuffers(1, &id);
}

tinygl::buffer::buffer_private::~buffer_private()
{
    glDeleteBuffers(1, &id);
}

tinygl::buffer::buffer(buffer::binding_target binding_target, buffer::usage_pattern usage_pattern) :
        p{std::make_unique<buffer_private>(binding_target, usage_pattern)}
{
}

tinygl::buffer::~buffer() = default;

tinygl::buffer::buffer(tinygl::buffer&& other) noexcept = default;

tinygl::buffer& tinygl::buffer::operator=(tinygl::buffer&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::buffer::bind()
{
    glBindBuffer(gl_enum(p->binding_target), p->id);
}

void tinygl::buffer::unbind()
{
    glBindBuffer(gl_enum(p->binding_target), 0);
}

void tinygl::buffer::create(std::size_t size, const void* data)
{
    glBufferData(
        gl_enum(p->binding_target),
        static_cast<GLsizeiptr>(size),
        data,
        gl_enum(p->usage_pattern)
    );
}

void tinygl::buffer::update(std::size_t offset, std::size_t size, void const* data)
{
    glBufferSubData(
        gl_enum(p->binding_target),
        static_cast<GLintptr>(offset),
        static_cast<GLsizeiptr>(size),
        data
    );
}
