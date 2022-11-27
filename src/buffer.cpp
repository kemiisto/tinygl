#include "tinygl/buffer.h"
#include <GL/glew.h>

struct tinygl::Buffer::BufferPrivate
{
    BufferPrivate(Buffer::Type t, Buffer::UsagePattern pattern);
    ~BufferPrivate();

    GLuint id = 0;
    Type type;
    UsagePattern usagePattern;
};

tinygl::Buffer::BufferPrivate::BufferPrivate(tinygl::Buffer::Type t, Buffer::UsagePattern pattern) :
        type{t},
        usagePattern{pattern}
{
    glGenBuffers(1, &id);
}

tinygl::Buffer::BufferPrivate::~BufferPrivate()
{
    glDeleteBuffers(1, &id);
}

tinygl::Buffer::Buffer(Buffer::Type type, Buffer::UsagePattern usagePattern) :
        p{std::make_unique<BufferPrivate>(type, usagePattern)}
{
}

tinygl::Buffer::~Buffer() = default;

tinygl::Buffer::Buffer(tinygl::Buffer&& other) noexcept = default;

tinygl::Buffer& tinygl::Buffer::operator=(tinygl::Buffer&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::Buffer::bind()
{
    glBindBuffer(static_cast<GLenum>(p->type), p->id);
}

void tinygl::Buffer::unbind()
{
    glBindBuffer(static_cast<GLenum>(p->type), 0);
}

void tinygl::Buffer::allocate(GLsizeiptr size)
{
    glBufferData(static_cast<GLenum>(p->type), size, nullptr, static_cast<GLenum>(p->usagePattern));
}

void tinygl::Buffer::fill(const void* data, GLsizeiptr size)
{
    glBufferData(static_cast<GLenum>(p->type), size, data, static_cast<GLenum>(p->usagePattern));
}

void tinygl::Buffer::write(GLintptr offset, void const* data, GLsizeiptr size)
{
    glBufferSubData(static_cast<GLenum>(p->type), offset, size, data);
}
