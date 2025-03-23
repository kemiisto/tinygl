#include "tinygl/tinygl.h"
#include "tinygl/window.h"

#include "imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <spdlog/spdlog.h>
#include <iostream>
#include <map>
#include <utility>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static const std::map<GLenum, std::string> gl_debug_source = {
    { GL_DEBUG_SOURCE_API            , "API"             },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM  , "Window System"   },
    { GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler" },
    { GL_DEBUG_SOURCE_THIRD_PARTY    , "Third Party"     },
    { GL_DEBUG_SOURCE_APPLICATION    , "Application"     },
    { GL_DEBUG_SOURCE_OTHER          , "Other"           }
};

static const std::map<GLenum, std::string> gl_debug_type = {
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

static const std::map<GLenum, std::string> gl_debug_severity = {
    { GL_DEBUG_SEVERITY_HIGH        , "High"         },
    { GL_DEBUG_SEVERITY_MEDIUM      , "Medium"       },
    { GL_DEBUG_SEVERITY_LOW         , "Low"          },
    { GL_DEBUG_SEVERITY_NOTIFICATION, "Notification" }
};

void GLAPIENTRY gl_debug_output(
        GLenum source,
        GLenum type,
        [[maybe_unused]] GLuint id,
        GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar* message,
        [[maybe_unused]] const void* user_param)
{
    std::cerr <<
        "================================[ OpenGL error ]================================\n" <<
        "Source: " << gl_debug_source.at(source) << '\n' <<
        "Type: " << gl_debug_type.at(type) << '\n' <<
        "Severity: " << gl_debug_severity.at(severity) << '\n' <<
        "Message: " << message << '\n' <<
        "================================[ Stack  Trace ]================================\n";
    std::cerr <<
        "================================================================================\n";
}

struct tinygl::window::window_private
{
    GLFWwindow* window = nullptr;
    key_callback key_callback;
    mouse_button_callback mouse_button_callback;
    float previous_time{};
    float current_time{};
    float delta_time{};
};

tinygl::window::window(int width, int height, std::string_view title, bool vsync) :
        p{std::make_unique<window_private>()}
{
    p->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    if (!p->window) {
        throw std::runtime_error("glfwCreateWindow() failed!");
    }
    glfwMakeContextCurrent(p->window);
    if (vsync) {
        glfwSwapInterval(1);
    }
    glfwSetFramebufferSizeCallback(p->window, framebuffer_size_callback);

    // see setKeyCallback() method
    glfwSetWindowUserPointer(p->window, reinterpret_cast<void*>(this));

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("glewInit() failed!");
    }

    spdlog::info("[tinygl::Window] ========== OpenGL properties ==========");
    const std::map<GLenum, std::string> gl_properties = {
        { GL_VENDOR,   "Vendor"   },
        { GL_RENDERER, "Renderer" },
        { GL_VERSION,  "Version"  }
    };
    for (const auto& [flag, desc] : gl_properties) {
        spdlog::info("[tinygl::Window] {}: {}", desc, tinygl::get_string(flag));
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    spdlog::info("[tinygl::Window] ========== OpenGL context properties ==========");
    const std::map<GLenum, std::string> gl_context_flags = {
        { GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT, "FORWARD_COMPATIBLE" },
        { GL_CONTEXT_FLAG_DEBUG_BIT, "DEBUG" },
        { GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT, "ROBUST_ACCESS" },
        { GL_CONTEXT_FLAG_NO_ERROR_BIT, "NO_ERROR" }
    };

    for (const auto& [flag, desc] : gl_context_flags) {
        spdlog::info("[tinygl::Window] [{}] {}", flags & flag ? "V" : " ", desc);
    }

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

tinygl::window::~window()
{
    if (p->window) {
        glfwDestroyWindow(p->window);
    }
}

tinygl::window::window(window&& other) noexcept = default;
tinygl::window& tinygl::window::operator=(window&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::window::run()
{
    init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(p->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto& io = ImGui::GetIO();
    auto font_size = 14.0f;
#ifndef __APPLE__
    auto* monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1.0f || yscale > 1.0f) {
        font_size *= xscale;
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(xscale);
    }
#endif
    io.Fonts->AddFontFromFileTTF("fonts/JetBrainsMono-Light.ttf", font_size, nullptr, nullptr);

    while (!glfwWindowShouldClose(p->window)) {
        process_input();

        draw();

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        draw_ui();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(p->window);
        glfwPollEvents();

        p->current_time = tinygl::get_time<float>();
        p->delta_time = (p->current_time - p->previous_time);
        p->previous_time = p->current_time;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

tinygl::keyboard::key_state tinygl::window::get_key(tinygl::keyboard::key key)
{
    return static_cast<tinygl::keyboard::key_state>(glfwGetKey(p->window, static_cast<int>(key)));
}

template<std::floating_point T>
std::tuple<T, T> tinygl::window::get_cursor_pos()
{
    double x, y;
    glfwGetCursorPos(p->window, &x, &y);
    if constexpr (std::is_same<T, double>::value) {
        return {x, y};
    } else {
        return {static_cast<T>(x), static_cast<T>(y)};
    }
}

template std::tuple<float, float> tinygl::window::get_cursor_pos<float>();
template std::tuple<double, double> tinygl::window::get_cursor_pos<double>();
template std::tuple<long double, long double> tinygl::window::get_cursor_pos<long double>();

std::tuple<int, int> tinygl::window::get_window_size()
{
    int width, height;
    glfwGetWindowSize(p->window, &width, &height);
    return {width, height};
}

std::tuple<int, int> tinygl::window::get_framebuffer_size()
{
    int width, height;
    glfwGetFramebufferSize(p->window, &width, &height);
    return {width, height};
}

template<std::floating_point T>
T tinygl::window::aspect_ratio()
{
    int width, height;
    glfwGetWindowSize(p->window, &width, &height);
    return static_cast<T>(width) / height;
}

template float tinygl::window::aspect_ratio<float>();
template double tinygl::window::aspect_ratio<double>();
template long double tinygl::window::aspect_ratio<long double>();

void tinygl::window::set_title(std::string_view title)
{
    glfwSetWindowTitle(p->window, title.data());
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void tinygl::window::set_should_close(bool should_close)
{
    glfwSetWindowShouldClose(p->window, should_close);
}

void tinygl::window::set_key_callback(tinygl::window::key_callback callback)
{
    // https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
    // https://www.glfw.org/faq.html#216---how-do-i-use-c-methods-as-callbacks
    p->key_callback = std::move(callback);
    auto glfw_key_callback = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<tinygl::window*>(glfwGetWindowUserPointer(w))->p->key_callback(
            static_cast<tinygl::keyboard::key>(key),
            scancode,
            static_cast<input::action>(action),
            static_cast<tinygl::input::modifier>(mods)
        );
    };
    glfwSetKeyCallback(p->window, glfw_key_callback);
}

void tinygl::window::set_mouse_button_callback(tinygl::window::mouse_button_callback callback)
{
    p->mouse_button_callback = std::move(callback);
    auto glfw_mouse_button_callback = [](GLFWwindow* w, int button, int action, int mods) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;
        static_cast<tinygl::window*>(glfwGetWindowUserPointer(w))->p->mouse_button_callback(
            static_cast<tinygl::mouse::button>(button),
            static_cast<input::action>(action),
            static_cast<tinygl::input::modifier>(mods)
        );
    };
    glfwSetMouseButtonCallback(p->window, glfw_mouse_button_callback);
}

float tinygl::window::delta_time() const
{
    return p->delta_time;
}
