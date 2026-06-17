#include <GL/glew.h>

#include "tinygl/tinygl.h"
#include <stdexcept>

namespace {
    constexpr GLenum gl_enum(tinygl::name name)
    {
        switch(name) {
        case tinygl::name::gl_vendor: return GL_VENDOR;
        case tinygl::name::gl_renderer: return GL_RENDERER;
        case tinygl::name::gl_version: return GL_VERSION;
        case tinygl::name::gl_shading_language_version: return GL_SHADING_LANGUAGE_VERSION;
        }
    }

    constexpr GLenum gl_enum(tinygl::mode mode)
    {
        switch (mode) {
        case tinygl::mode::gl_points: return GL_POINTS;
        case tinygl::mode::gl_line_strip: return GL_LINE_STRIP;
        case tinygl::mode::gl_line_loop: return GL_LINE_LOOP;
        case tinygl::mode::gl_lines: return GL_LINES;
        case tinygl::mode::gl_line_strip_adjacency: return GL_LINE_STRIP_ADJACENCY;
        case tinygl::mode::gl_lines_adjacency: return GL_LINES_ADJACENCY;
        case tinygl::mode::gl_triangle_strip: return GL_TRIANGLE_STRIP;
        case tinygl::mode::gl_triangle_fan: return GL_TRIANGLE_FAN;
        case tinygl::mode::gl_triangles: return GL_TRIANGLES;
        case tinygl::mode::gl_triangle_strip_adjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
        case tinygl::mode::gl_triangles_adjacency: return GL_TRIANGLES_ADJACENCY;
        case tinygl::mode::gl_patches: return GL_PATCHES;
        }
    }

    constexpr GLenum gl_enum(tinygl::capability capability)
    {
        switch(capability) {
        case tinygl::capability::gl_blend: return GL_BLEND;
        case tinygl::capability::gl_clip_distance0: return GL_CLIP_DISTANCE0;
        case tinygl::capability::gl_clip_distance1: return GL_CLIP_DISTANCE1;
        case tinygl::capability::gl_clip_distance2: return GL_CLIP_DISTANCE2;
        case tinygl::capability::gl_clip_distance3: return GL_CLIP_DISTANCE3;
        case tinygl::capability::gl_clip_distance4: return GL_CLIP_DISTANCE4;
        case tinygl::capability::gl_clip_distance5: return GL_CLIP_DISTANCE5;
        case tinygl::capability::gl_color_logic_op: return GL_COLOR_LOGIC_OP;
        case tinygl::capability::gl_cull_face: return GL_CULL_FACE;
        case tinygl::capability::gl_debug_output: return GL_DEBUG_OUTPUT;
        case tinygl::capability::gl_debug_output_synchronous: return GL_DEBUG_OUTPUT_SYNCHRONOUS;
        case tinygl::capability::gl_depth_clamp: return GL_DEPTH_CLAMP;
        case tinygl::capability::gl_depth_test: return GL_DEPTH_TEST;
        case tinygl::capability::gl_dither: return GL_DITHER;
        case tinygl::capability::gl_framebuffer_srgb: return GL_FRAMEBUFFER_SRGB;
        case tinygl::capability::gl_line_smooth: return GL_LINE_SMOOTH;
        case tinygl::capability::gl_multisample: return GL_MULTISAMPLE;
        case tinygl::capability::gl_polygon_offset_fill: return GL_POLYGON_OFFSET_FILL;
        case tinygl::capability::gl_polygon_offset_line: return GL_POLYGON_OFFSET_LINE;
        case tinygl::capability::gl_polygon_offset_point: return GL_POLYGON_OFFSET_POINT;
        case tinygl::capability::gl_polygon_smooth: return GL_POLYGON_SMOOTH;
        case tinygl::capability::gl_primitive_restart: return GL_PRIMITIVE_RESTART;
        case tinygl::capability::gl_primitive_restart_fixed_index: return GL_PRIMITIVE_RESTART_FIXED_INDEX;
        case tinygl::capability::gl_rasterizer_discard: return GL_RASTERIZER_DISCARD;
        case tinygl::capability::gl_sample_alpha_to_coverage: return GL_SAMPLE_ALPHA_TO_COVERAGE;
        case tinygl::capability::gl_sample_alpha_to_one: return GL_SAMPLE_ALPHA_TO_ONE;
        case tinygl::capability::gl_sample_coverage: return GL_SAMPLE_COVERAGE;
        case tinygl::capability::gl_sample_shading: return GL_SAMPLE_SHADING;
        case tinygl::capability::gl_sample_mask: return GL_SAMPLE_MASK;
        case tinygl::capability::gl_scissor_test: return GL_SCISSOR_TEST;
        case tinygl::capability::gl_stencil_test: return GL_STENCIL_TEST;
        case tinygl::capability::gl_texture_cube_map_seamless: return GL_TEXTURE_CUBE_MAP_SEAMLESS;
        case tinygl::capability::gl_program_point_size: return GL_PROGRAM_POINT_SIZE;
        }
    }

    constexpr GLenum gl_enum(tinygl::depth_func depth_func)
    {
        switch (depth_func) {
        case tinygl::depth_func::gl_never: return GL_NEVER;
        case tinygl::depth_func::gl_less: return GL_LESS;
        case tinygl::depth_func::gl_equal: return GL_EQUAL;
        case tinygl::depth_func::gl_lequal: return GL_LEQUAL;
        case tinygl::depth_func::gl_greater: return GL_GREATER;
        case tinygl::depth_func::gl_notequal: return GL_NOTEQUAL;
        case tinygl::depth_func::gl_gequal: return GL_GEQUAL;
        case tinygl::depth_func::gl_always: return GL_ALWAYS;
        }
    }
}

void tinygl::gl_clear_color(const tinygl::color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void tinygl::gl_clear(buffer_bit buffer_bit)
{
    glClear(static_cast<GLenum>(buffer_bit));
}

void tinygl::gl_point_size(float size)
{
    glPointSize(size);
}

void tinygl::gl_draw_arrays(mode mode, int32_t first, int32_t count)
{
    glDrawArrays(gl_enum(mode), first, count);
}

void tinygl::gl_enable(capability capability)
{
    glEnable(gl_enum(capability));
}

void tinygl::gl_depth_func(depth_func depth_func)
{
    glDepthFunc(gl_enum(depth_func));
}

void tinygl::init(int major, int minor)
{
    // Force GLFW to bypass Wayland and use X11
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

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

const char* tinygl::get_string(name name)
{
    return reinterpret_cast<const char*>(glGetString(gl_enum(name)));
}

void tinygl::check_opengl_errors()
{
    auto error = glGetError();
    while (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << '\n';
        error = glGetError();
    }
}
