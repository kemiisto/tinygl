#include "tinygl/tinygl.h"
#include "tinygl/window.h"

#include <spdlog/spdlog.h>
#include <iostream>
#include <map>
#include <utility>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

static const std::map<GLenum, std::string> glDebugSource = {
    { GL_DEBUG_SOURCE_API            , "API"             },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM  , "Window System"   },
    { GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler" },
    { GL_DEBUG_SOURCE_THIRD_PARTY    , "Third Party"     },
    { GL_DEBUG_SOURCE_APPLICATION    , "Application"     },
    { GL_DEBUG_SOURCE_OTHER          , "Other"           }
};

static const std::map<GLenum, std::string> glDebugType = {
    { GL_DEBUG_TYPE_ERROR              , "Error"               },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behavior" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR , "Undefined Behavior"  },
    { GL_DEBUG_TYPE_PORTABILITY        , "Portability"         },
    { GL_DEBUG_TYPE_PERFORMANCE        , "Performance"         },
    { GL_DEBUG_TYPE_MARKER             , "Marker"              },
    { GL_DEBUG_TYPE_PUSH_GROUP         , "Push Group"          },
    { GL_DEBUG_TYPE_POP_GROUP          , "Pop Group"           },
    { GL_DEBUG_TYPE_OTHER              , "Other"               }
};

static const std::map<GLenum, std::string> glDebugSeverity = {
    { GL_DEBUG_SEVERITY_HIGH        , "High"         },
    { GL_DEBUG_SEVERITY_MEDIUM      , "Medium"       },
    { GL_DEBUG_SEVERITY_LOW         , "Low"          },
    { GL_DEBUG_SEVERITY_NOTIFICATION, "Notification" }
};

void GLAPIENTRY glDebugOutput(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
{
    std::cerr <<
        "================================[ OpenGL error ]================================\n" <<
        "Source: " << glDebugSource.at(source) << '\n' <<
        "Type: " << glDebugType.at(type) << '\n' <<
        "Severity: " << glDebugSeverity.at(severity) << '\n' <<
        "Message: " << message << '\n' <<
        "================================[ Stack  Trace ]================================\n";
    std::cerr <<
        "================================================================================\n";
}

struct tinygl::Window::WindowPrivate
{
    GLFWwindow* window = nullptr;
    KeyCallback keyCallback;
};

tinygl::Window::Window(int width, int height, const std::string& title) :
        p{std::make_unique<WindowPrivate>()}
{
    /**
     * int glfwInit()
     *
     * Before most GLFW functions can be used, GLFW must be initialized by glfwInit call.
     * And before an application terminates GLFW should be terminated by glfwTerminate call.
     *
     * Note though, that if glfwInit fails, it calls glfwTerminate before returning.
     * So you should call glfwTerminate before the application exits only if glfwInit succeeds.
     *
     * Returns GLFW_TRUE (1) if successful, or GLFW_FALSE (0) if an error occurred.
     */
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit() failed!");
    }
    /**
     * void glfwWindowHint(int hint, int value)
     *
     * Sets hints for the next call to glfwCreateWindow.
     * The first argument is the option we want to configure.
     * The second argument is an integer value of the option.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /**
     * https://www.khronos.org/opengl/wiki/OpenGL_Context#Forward_compatibility
     *
     * Forward compatibility will cause, for the given profile, all functionality marked "deprecated" to be removed.
     * Recommendation: You should use the forward compatibility bit only if you need compatibility with macOS.
     * That API requires the forward compatibility bit to create any core profile context.
     */
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    /**
     * GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
     *
     * Creates a window and its associated OpenGL or OpenGL ES context.
     * Successful creation does not change which context is current.
     * Before you can use the newly created context, you need to make it current.
     *
     * Returns the handle of the created window, or NULL if an error occurred.
     */
    p->window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!p->window) {
        throw std::runtime_error("glfwCreateWindow() failed!");
    }
    glfwMakeContextCurrent(p->window);
    glfwSetFramebufferSizeCallback(p->window, framebufferSizeCallback);

    // see setKeyCallback() method
    glfwSetWindowUserPointer(p->window, reinterpret_cast<void*>(this));

    /**
     * https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#Initialization_of_GLEW_1.13.0_and_earlier
     *
     * GLEW up to version 1.13.0 has a problem with core contexts.
     * The only fix for earlier versions is to set glewExperimental to GL_TRUE before calling glewInit().
     */
    glewExperimental = GL_TRUE;
    /**
     * glewInit creates a valid OpenGL rendering context and initializes the extension entry points.
     * Returns GLEW_OK if succeeded so you can use the available extensions as well as core OpenGL functionality.
     * Somewhat strangely GLEW_OK is defined to be 0, so the idiomatic if (!glewInit()) {...} won't work!
     * if (glewInit()) {...} will of course, but one would better use a more lengthy but explicit condition.
     */
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("glewInit() failed!");
    }

    spdlog::info("[tinygl::Window] ========== OpenGL properties ==========");
    const std::map<GLenum, std::string> glProperties = {
        { GL_VENDOR,   "Vendor"   },
        { GL_RENDERER, "Renderer" },
        { GL_VERSION,  "Version"  }
    };
    for (const auto& [flag, desc] : glProperties) {
        spdlog::info("[tinygl::Window] {}: {}", desc, tinygl::getString(flag));
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    spdlog::info("[tinygl::Window] ========== OpenGL context properties ==========");
    const std::map<GLenum, std::string> glContextFlags = {
        { GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT, "FORWARD_COMPATIBLE" },
        { GL_CONTEXT_FLAG_DEBUG_BIT, "DEBUG" },
        { GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT, "ROBUST_ACCESS" },
        { GL_CONTEXT_FLAG_NO_ERROR_BIT, "NO_ERROR" }
    };

    for (const auto& [flag, desc] : glContextFlags) {
        spdlog::info("[tinygl::Window] [{}] {}", flags & flag ? "V" : " ", desc);
    }

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

tinygl::Window::~Window()
{
    if (p->window) {
        /**
         * void glfwDestroyWindow(GLFWwindow* window)
         *
         * Destroys the specified window and its context.
         */
        glfwDestroyWindow(p->window);
    }
}

tinygl::Window::Window(Window&& other) noexcept = default;
tinygl::Window& tinygl::Window::operator=(Window&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::Window::run()
{
    init();
    while (!glfwWindowShouldClose(p->window)) {
        processInput();
        draw();
        glfwSwapBuffers(p->window);
        glfwPollEvents();
    }
}

tinygl::KeyState tinygl::Window::getKey(tinygl::Key key)
{
    return static_cast<KeyState>(glfwGetKey(p->window, static_cast<int>(key)));
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void tinygl::Window::setShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(p->window, shouldClose);
}

void tinygl::Window::setKeyCallback(tinygl::Window::KeyCallback callback)
{
    // https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
    // https://www.glfw.org/faq.html#216---how-do-i-use-c-methods-as-callbacks
    p->keyCallback = std::move(callback);
    auto glfwKeyCallback = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<tinygl::Window*>(glfwGetWindowUserPointer(w))->p->keyCallback(
            static_cast<Key>(key), scancode, static_cast<KeyAction>(action), static_cast<Modifier>(mods)
        );
    };
    glfwSetKeyCallback(p->window, glfwKeyCallback);
}
