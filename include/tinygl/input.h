#ifndef TINYGL_INPUT_H
#define TINYGL_INPUT_H

#include "tinygl/bitmask_operators.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tinygl::input
{
    enum class action {
        release = GLFW_RELEASE,
        press = GLFW_PRESS,
        repeat = GLFW_REPEAT
    };

    enum class modifier {
        shift = GLFW_MOD_SHIFT,
        control = GLFW_MOD_CONTROL,
        alt = GLFW_MOD_ALT,
        super = GLFW_MOD_SUPER,
    };
}

template<>
struct enable_bitmask_operators<tinygl::input::modifier> {
    static const bool enable = true;
};

#endif // TINYGL_INPUT_H
