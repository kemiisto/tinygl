#include "tinygl/vertex_array_object.h"
#include <GL/glew.h>
#include <stdexcept>

struct tinygl::vertex_array_object::vertex_array_object_private
{
    vertex_array_object_private();
    ~vertex_array_object_private();

    GLuint id = 0;
};

tinygl::vertex_array_object::vertex_array_object_private::vertex_array_object_private()
{
    glGenVertexArrays(1, &id);
}

tinygl::vertex_array_object::vertex_array_object_private::~vertex_array_object_private()
{
    glDeleteVertexArrays(1, &id);
}

tinygl::vertex_array_object::vertex_array_object() :
        p{std::make_unique<vertex_array_object_private>()}
{
}

tinygl::vertex_array_object::~vertex_array_object() = default;

tinygl::vertex_array_object::vertex_array_object(vertex_array_object&& other) noexcept = default;

tinygl::vertex_array_object& tinygl::vertex_array_object::operator=(vertex_array_object&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::vertex_array_object::bind()
{
    if (!p->id) {
        throw std::runtime_error("tinygl::vertex_array_object::bind(): vao not created!");
    }
    glBindVertexArray(p->id);
}

void tinygl::vertex_array_object::unbind()
{
    if (!p->id) {
        throw std::runtime_error("tinygl::vertex_array_object::unbind(): vao not created!");
    }
    glBindVertexArray(0);
}

void tinygl::vertex_array_object::set_attribute_array(
        int location, int tuple_size, GLenum type, bool normalize, int stride, int offset)
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
    glVertexAttribPointer(location, tuple_size, type, normalize, stride, reinterpret_cast<void*>(offset));
}

void tinygl::vertex_array_object::enable_attribute_array(int location)
{
    glEnableVertexAttribArray(location);
}

