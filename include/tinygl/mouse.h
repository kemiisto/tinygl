#ifndef TINYGL_MOUSE_H
#define TINYGL_MOUSE_H

#include "tinygl/bitmask_operators.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tinygl::mouse {
    enum class button {
        one = GLFW_MOUSE_BUTTON_1,
        two = GLFW_MOUSE_BUTTON_2,
        three = GLFW_MOUSE_BUTTON_3,
        four = GLFW_MOUSE_BUTTON_4,
        five = GLFW_MOUSE_BUTTON_5,
        six = GLFW_MOUSE_BUTTON_6,
        seven = GLFW_MOUSE_BUTTON_7,
        eight = GLFW_MOUSE_BUTTON_8,
        last = GLFW_MOUSE_BUTTON_LAST,
        left = GLFW_MOUSE_BUTTON_LEFT,
        right = GLFW_MOUSE_BUTTON_RIGHT,
        middle = GLFW_MOUSE_BUTTON_MIDDLE
    };
}

#endif // TINYGL_MOUSE_H
