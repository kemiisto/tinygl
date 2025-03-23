#ifndef TINYGL_VERTEX_ARRAY_OBJECT_H
#define TINYGL_VERTEX_ARRAY_OBJECT_H

#include <GL/glew.h>
#include <memory>

namespace tinygl
{
    class vertex_array_object final
    {
    public:
        vertex_array_object();
        ~vertex_array_object();

        vertex_array_object(vertex_array_object&& other) noexcept;
        vertex_array_object& operator=(vertex_array_object&& other) noexcept;

        void bind();
        void unbind();

        void set_attribute_array(
            int location, int tuple_size, GLenum type, bool normalize, int stride = 0, int offset = 0);
        void enable_attribute_array(int location);

    private:
        struct vertex_array_object_private;
        std::unique_ptr<vertex_array_object_private> p;
    };
}

#endif // TINYGL_VERTEX_ARRAY_OBJECT_H
