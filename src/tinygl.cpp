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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
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

template<std::floating_point T>
T tinygl::get_time()
{
    if constexpr (std::is_same<T, double>::value) {
        return glfwGetTime();
    } else {
        return static_cast<T>(glfwGetTime());
    }
}

template float tinygl::get_time<float>();
template double tinygl::get_time<double>();
template long double tinygl::get_time<long double>();

const char* tinygl::get_string(GLenum name)
{
    return reinterpret_cast<const char*>(glGetString(name));
}