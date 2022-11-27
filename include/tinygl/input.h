#ifndef TINYGL_INPUT_H
#define TINYGL_INPUT_H

#include "tinygl/bitmask_operators.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tinygl::input
{
    enum class Action {
        Release = GLFW_RELEASE,
        Press = GLFW_PRESS,
        Repeat = GLFW_REPEAT
    };

    enum class Modifier {
        Shift = GLFW_MOD_SHIFT,
        Control = GLFW_MOD_CONTROL,
        Alt = GLFW_MOD_ALT,
        Super = GLFW_MOD_SUPER,
    };
}

template<>
struct enable_bitmask_operators<tinygl::input::Modifier> {
    static const bool enable = true;
};

#endif // TINYGL_INPUT_H
