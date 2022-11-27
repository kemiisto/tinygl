#ifndef TINYGL_MOUSE_H
#define TINYGL_MOUSE_H

#include "tinygl/bitmask_operators.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tinygl::mouse
{
    enum class Button
    {
        One = GLFW_MOUSE_BUTTON_1,
        Two = GLFW_MOUSE_BUTTON_2,
        Three = GLFW_MOUSE_BUTTON_3,
        Four = GLFW_MOUSE_BUTTON_4,
        Five = GLFW_MOUSE_BUTTON_5,
        Six = GLFW_MOUSE_BUTTON_6,
        Seven = GLFW_MOUSE_BUTTON_7,
        Eight = GLFW_MOUSE_BUTTON_8,
        Last = GLFW_MOUSE_BUTTON_LAST,
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE
    };
}

#endif // TINYGL_MOUSE_H
