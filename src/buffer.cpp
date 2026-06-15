#include "tinygl/buffer.h"
#include <GL/glew.h>

namespace {
    constexpr GLenum gl_enum(tinygl::buffer::type type) {
        switch(type) {
        case tinygl::buffer::type::vertex_buffer: return GL_ARRAY_BUFFER;
        case tinygl::buffer::type::index_buffer: return GL_ELEMENT_ARRAY_BUFFER;
        }
    }

    constexpr GLenum gl_enum(tinygl::buffer::usage_pattern usage_pattern) {
        switch(usage_pattern) {
        case tinygl::buffer::usage_pattern::stream_draw: return GL_STREAM_DRAW;
        case tinygl::buffer::usage_pattern::stream_read: return GL_STREAM_READ;
        case tinygl::buffer::usage_pattern::stream_copy: return GL_STREAM_COPY;
        case tinygl::buffer::usage_pattern::static_draw: return GL_STATIC_DRAW;
        case tinygl::buffer::usage_pattern::static_read: return GL_STATIC_READ;
        case tinygl::buffer::usage_pattern::static_copy: return GL_STATIC_COPY;
        case tinygl::buffer::usage_pattern::dynamic_draw: return GL_DYNAMIC_DRAW;
        case tinygl::buffer::usage_pattern::dynamic_read: return GL_DYNAMIC_READ;
        case tinygl::buffer::usage_pattern::dynamic_copy: return GL_DYNAMIC_COPY;
        }
    }
}

struct tinygl::buffer::buffer_private
{
    buffer_private(buffer::type t, buffer::usage_pattern pattern);
    ~buffer_private();

    GLuint id = 0;
    type type;
    usage_pattern usage_pattern;
};

tinygl::buffer::buffer_private::buffer_private(tinygl::buffer::type t, buffer::usage_pattern pattern) :
        type{t},
        usage_pattern{pattern}
{
    glGenBuffers(1, &id);
}

tinygl::buffer::buffer_private::~buffer_private()
{
    glDeleteBuffers(1, &id);
}

tinygl::buffer::buffer(buffer::type type, buffer::usage_pattern usage_pattern) :
        p{std::make_unique<buffer_private>(type, usage_pattern)}
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
    glBindBuffer(gl_enum(p->type), p->id);
}

void tinygl::buffer::unbind()
{
    glBindBuffer(gl_enum(p->type), 0);
}

void tinygl::buffer::create(std::size_t size, const void* data)
{
    glBufferData(
        gl_enum(p->type),
        static_cast<GLsizeiptr>(size),
        data,
        gl_enum(p->usage_pattern)
    );
}

void tinygl::buffer::update(std::size_t offset, std::size_t size, void const* data)
{
    glBufferSubData(
        gl_enum(p->type),
        static_cast<GLintptr>(offset),
        static_cast<GLsizeiptr>(size),
        data
    );
}
