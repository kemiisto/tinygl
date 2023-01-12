#include "tinygl/vertex_array_object.h"
#include <GL/glew.h>
#include <stdexcept>

struct tinygl::VertexArrayObject::VertexArrayObjectPrivate
{
    VertexArrayObjectPrivate();
    ~VertexArrayObjectPrivate();

    GLuint id = 0;
};

tinygl::VertexArrayObject::VertexArrayObjectPrivate::VertexArrayObjectPrivate()
{
    glGenVertexArrays(1, &id);
}

tinygl::VertexArrayObject::VertexArrayObjectPrivate::~VertexArrayObjectPrivate()
{
    glDeleteVertexArrays(1, &id);
}

tinygl::VertexArrayObject::VertexArrayObject() :
        p{std::make_unique<VertexArrayObjectPrivate>()}
{
}

tinygl::VertexArrayObject::~VertexArrayObject() = default;

tinygl::VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept = default;

tinygl::VertexArrayObject& tinygl::VertexArrayObject::operator=(VertexArrayObject&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::VertexArrayObject::bind()
{
    if (!p->id) {
        throw std::runtime_error("tinygl::VertexArrayObject::bind(): vao not created!");
    }
    glBindVertexArray(p->id);
}

void tinygl::VertexArrayObject::unbind()
{
    if (!p->id) {
        throw std::runtime_error("tinygl::VertexArrayObject::unbind(): vao not created!");
    }
    glBindVertexArray(0);
}

void tinygl::VertexArrayObject::setAttributeArray(
        int location, int tupleSize, GLenum type, bool normalize, int stride, int offset)
{
    /**
     * https://www.khronos.org/opengl/wiki/Vertex_Specification
     * https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_vertex_buffer_object.txt
     *
     * Is it legal C to use pointers as offsets?
     *
     * We haven't come to any definitive conclusion about this. The proposal is to convert to pointer as:
     *     pointer = (char *)NULL + offset;
     * Varying opinions have been expressed as to whether this is legal,
     * although no one could provide an example of a real system where any problems would occur.
     *
     * See also:
     * https://stackoverflow.com/questions/26201840/int-to-void-avoiding-c-style-cast
     * https://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
     * https://stackoverflow.com/questions/58679610/how-to-cast-an-integer-to-a-void-without-violating-c-core-guidelines
     */

    // Apple clang 14.0.0
    // warning: arithmetic on a null pointer treated as a cast from integer to pointer is a GNU extension [-Wnull-pointer-arithmetic]
    // glVertexAttribPointer(location, tupleSize, type, normalize, stride, static_cast<char*>(nullptr) + offset);

    // Apple clang 14.0.0
    // No warnings.
    glVertexAttribPointer(location, tupleSize, type, normalize, stride, reinterpret_cast<void*>(offset));
}

void tinygl::VertexArrayObject::enableAttributeArray(int location)
{
    glEnableVertexAttribArray(location);
}

