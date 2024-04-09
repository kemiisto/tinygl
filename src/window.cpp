#include "tinygl/tinygl.h"
#include "tinygl/window.h"

#include "imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

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
        [[maybe_unused]] GLuint id,
        GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar* message,
        [[maybe_unused]] const void* userParam)
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
    MouseButtonCallback mouseButtonCallback;
    float previousTime{};
    float currentTime{};
    float deltaTime{};
};

tinygl::Window::Window(int width, int height, std::string_view title, bool vsync) :
        p{std::make_unique<WindowPrivate>()}
{
    p->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    if (!p->window) {
        throw std::runtime_error("glfwCreateWindow() failed!");
    }
    glfwMakeContextCurrent(p->window);
    if (vsync) {
        glfwSwapInterval(1);
    }
    glfwSetFramebufferSizeCallback(p->window, framebufferSizeCallback);

    // see setKeyCallback() method
    glfwSetWindowUserPointer(p->window, reinterpret_cast<void*>(this));

    glewExperimental = GL_TRUE;
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(p->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto& io = ImGui::GetIO();
    auto fontSize = 14.0f;
#ifndef __APPLE__
    auto* monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1.0f || yscale > 1.0f) {
        fontSize *= xscale;
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(xscale);
    }
#endif
    io.Fonts->AddFontFromFileTTF("fonts/JetBrainsMono-Light.ttf", fontSize, nullptr, nullptr);

    while (!glfwWindowShouldClose(p->window)) {
        processInput();

        draw();

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUi();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(p->window);
        glfwPollEvents();

        p->currentTime = tinygl::getTime<float>();
        p->deltaTime = (p->currentTime - p->previousTime);
        p->previousTime = p->currentTime;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

tinygl::keyboard::KeyState tinygl::Window::getKey(tinygl::keyboard::Key key)
{
    return static_cast<tinygl::keyboard::KeyState>(glfwGetKey(p->window, static_cast<int>(key)));
}

template<std::floating_point T>
std::tuple<T, T> tinygl::Window::getCursorPos()
{
    double x, y;
    glfwGetCursorPos(p->window, &x, &y);
    if constexpr (std::is_same<T, double>::value) {
        return {x, y};
    } else {
        return {static_cast<T>(x), static_cast<T>(y)};
    }
}

template std::tuple<float, float> tinygl::Window::getCursorPos<float>();
template std::tuple<double, double> tinygl::Window::getCursorPos<double>();
template std::tuple<long double, long double> tinygl::Window::getCursorPos<long double>();

std::tuple<int, int> tinygl::Window::getWindowSize()
{
    int width, height;
    glfwGetWindowSize(p->window, &width, &height);
    return {width, height};
}

std::tuple<int, int> tinygl::Window::getFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(p->window, &width, &height);
    return {width, height};
}

template<std::floating_point T>
T tinygl::Window::aspectRatio()
{
    int width, height;
    glfwGetWindowSize(p->window, &width, &height);
    return static_cast<T>(width) / height;
}

template float tinygl::Window::aspectRatio<float>();
template double tinygl::Window::aspectRatio<double>();
template long double tinygl::Window::aspectRatio<long double>();

void tinygl::Window::setTitle(std::string_view title)
{
    glfwSetWindowTitle(p->window, title.data());
}

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
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
            static_cast<tinygl::keyboard::Key>(key),
            scancode,
            static_cast<input::Action>(action),
            static_cast<tinygl::input::Modifier>(mods)
        );
    };
    glfwSetKeyCallback(p->window, glfwKeyCallback);
}

void tinygl::Window::setMouseButtonCallback(tinygl::Window::MouseButtonCallback callback)
{
    p->mouseButtonCallback = std::move(callback);
    auto glfwMouseButtonCallback = [](GLFWwindow* w, int button, int action, int mods) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;
        static_cast<tinygl::Window*>(glfwGetWindowUserPointer(w))->p->mouseButtonCallback(
            static_cast<tinygl::mouse::Button>(button),
            static_cast<input::Action>(action),
            static_cast<tinygl::input::Modifier>(mods)
        );
    };
    glfwSetMouseButtonCallback(p->window, glfwMouseButtonCallback);
}

float tinygl::Window::deltaTime() const
{
    return p->deltaTime;
}
