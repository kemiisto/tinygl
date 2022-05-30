#ifndef TINYGL_VERTEX_ARRAY_OBJECT_H
#define TINYGL_VERTEX_ARRAY_OBJECT_H

#include <GL/glew.h>
#include <memory>

namespace tinygl
{
    class VertexArrayObject final
    {
    public:
        VertexArrayObject();
        ~VertexArrayObject();

        VertexArrayObject(VertexArrayObject&& other) noexcept;
        VertexArrayObject& operator=(VertexArrayObject&& other) noexcept;

        void bind();
        void unbind();

        void setAttributeArray(
            int location, int tupleSize, GLenum type, bool normalize, int stride = 0, int offset = 0);
        void enableAttributeArray(int location);

    private:
        struct VertexArrayObjectPrivate;
        std::unique_ptr<VertexArrayObjectPrivate> p;
    };
}

#endif // TINYGL_VERTEX_ARRAY_OBJECT_H
