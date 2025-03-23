#ifndef TINYGL_TINYGL_H
#define TINYGL_TINYGL_H

#include "tinygl/buffer.h"
#include "tinygl/shader.h"
#include "tinygl/shader_program.h"
#include "tinygl/texture.h"
#include "tinygl/vertex_array_object.h"
#include "tinygl/window.h"
#include "tinygl/keyboard.h"
#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#include <tinyla/vec.hpp>
#include "imgui.h"

namespace tinygl
{
    void init(int major, int minor);
    void terminate();

    template<std::floating_point T>
    T get_time();

    const char* get_string(GLenum name);
}

extern template float tinygl::get_time<float>();
extern template double tinygl::get_time<double>();
extern template long double tinygl::get_time<long double>();

#endif // TINYGL_TINYGL_H
