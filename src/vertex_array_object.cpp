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
     * Here we have to convert the last parameter from `int` to `void*` to send it to `glVertexAttribPointer`.
     * This can be done essentially using:
     *   [1] `static_cast<char*>(nullptr) + offset`
     *   [2] `reinterpret_cast<void*>(offset)`
     *
     * On 3 major C++ compilers (clang 14, GCC 12, MSVC 19):
     *   [1] results in warning with clang only
     *   warning: arithmetic on a null pointer treated as a cast from integer to pointer is a GNU extension [-Wnull-pointer-arithmetic]
     *   [2] results in warning with MSVC only
     *   warning C4312: 'reinterpret_cast': conversion from 'int' to 'void *' of greater size
     *
     * Now, pointer arithmetic on `(char*)nullptr` in [1] is certainly UB.
     * And although no one could provide an example of a real system where any problems would occur, I lean towards [2].
     *
     * The OpenGL Wiki also suggests using `reinterpret_cast`.
     * https://www.khronos.org/opengl/wiki/Vertex_Specification
     */
#ifdef _MSC_VER
#pragma warning(disable: 4312) // possible loss of data
#endif
    glVertexAttribPointer(location, tupleSize, type, normalize, stride, reinterpret_cast<void*>(offset));
}

void tinygl::VertexArrayObject::enableAttributeArray(int location)
{
    glEnableVertexAttribArray(location);
}

