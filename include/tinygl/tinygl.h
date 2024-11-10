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
    T getTime();

    const char* getString(GLenum name);
}

extern template float tinygl::getTime<float>();
extern template double tinygl::getTime<double>();
extern template long double tinygl::getTime<long double>();

#endif // TINYGL_TINYGL_H
