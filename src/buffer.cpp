#include "tinygl/buffer.h"
#include <GL/glew.h>

struct tinygl::buffer::buffer_private
{
    buffer_private(buffer::type t, buffer::usage_pattern pattern);
    ~buffer_private();

    GLuint id = 0;
    type type;
    usage_pattern buffer_usage_pattern;
};

tinygl::buffer::buffer_private::buffer_private(tinygl::buffer::type t, buffer::usage_pattern pattern) :
        type{t},
        buffer_usage_pattern{pattern}
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
    glBindBuffer(static_cast<GLenum>(p->type), p->id);
}

void tinygl::buffer::unbind()
{
    glBindBuffer(static_cast<GLenum>(p->type), 0);
}

void tinygl::buffer::create(std::size_t size, const void* data)
{
    glBufferData(
        static_cast<GLenum>(p->type),
        static_cast<GLsizeiptr>(size),
        data,
        static_cast<GLenum>(p->buffer_usage_pattern)
    );
}

void tinygl::buffer::update(std::size_t offset, std::size_t size, void const* data)
{
    glBufferSubData(
        static_cast<GLenum>(p->type),
        static_cast<GLintptr>(offset),
        static_cast<GLsizeiptr>(size),
        data
    );
}
