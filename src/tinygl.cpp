#include "tinygl/tinygl.h"

float tinygl::getTime()
{
    return static_cast<float>(glfwGetTime());
}

const char* tinygl::getString(GLenum name)
{
    return reinterpret_cast<const char*>(glGetString(name));
}