#include "tinygl/tinygl.h"
#include <stdexcept>

void tinygl::init(int major, int minor)
{
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit() failed!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    auto* monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1.0f || yscale > 1.0f) {
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#endif
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}

void tinygl::terminate()
{
    glfwTerminate();
}

float tinygl::getTime()
{
    return static_cast<float>(glfwGetTime());
}

const char* tinygl::getString(GLenum name)
{
    return reinterpret_cast<const char*>(glGetString(name));
}