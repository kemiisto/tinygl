#ifndef TINYGL_TINYGL_H
#define TINYGL_TINYGL_H

#include "tinygl/bitmask_operators.h"
#include "tinygl/buffer.h"
#include "tinygl/color.h"
#include "tinygl/data_types.h"
#include "tinygl/keyboard.h"
#include "tinygl/shader.h"
#include "tinygl/shader_program.h"
#include "tinygl/texture.h"
#include "tinygl/vertex_array_object.h"
#include "tinygl/window.h"
#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#include <tinyla/vec.hpp>
#include "imgui.h"

namespace tinygl
{
    void gl_clear_color(const color& color);

    enum class buffer_bit : std::uint32_t {
        gl_color_buffer_bit   = 0x00004000,  // GL_COLOR_BUFFER_BIT
        gl_depth_buffer_bit   = 0x00000100,  // GL_DEPTH_BUFFER_BIT
        gl_stencil_buffer_bit = 0x00000400   // GL_STENCIL_BUFFER_BIT
    };
    void gl_clear(buffer_bit buffer_bit);

    void gl_point_size(float size);

    enum class mode : std::uint32_t {
        gl_points,
        gl_line_strip,
        gl_line_loop,
        gl_lines,
        gl_line_strip_adjacency,
        gl_lines_adjacency,
        gl_triangle_strip,
        gl_triangle_fan,
        gl_triangles,
        gl_triangle_strip_adjacency,
        gl_triangles_adjacency,
        gl_patches
    };
    void gl_draw_arrays(mode mode, std::int32_t first, std::int32_t count);
    void gl_draw_arrays_instanced(mode mode, std::int32_t first, std::int32_t count, std::int32_t instance_count);
    void gl_draw_elements(mode mode, std::int32_t count, data_type type, const void* indices);

    enum struct capability : std::uint32_t {
        gl_blend,
        gl_clip_distance0,
        gl_clip_distance1,
        gl_clip_distance2,
        gl_clip_distance3,
        gl_clip_distance4,
        gl_clip_distance5,
        gl_color_logic_op,
        gl_cull_face,
        gl_debug_output,
        gl_debug_output_synchronous,
        gl_depth_clamp,
        gl_depth_test,
        gl_dither,
        gl_framebuffer_srgb,
        gl_line_smooth,
        gl_multisample,
        gl_polygon_offset_fill,
        gl_polygon_offset_line,
        gl_polygon_offset_point,
        gl_polygon_smooth,
        gl_primitive_restart,
        gl_primitive_restart_fixed_index,
        gl_rasterizer_discard,
        gl_sample_alpha_to_coverage,
        gl_sample_alpha_to_one,
        gl_sample_coverage,
        gl_sample_shading,
        gl_sample_mask,
        gl_scissor_test,
        gl_stencil_test,
        gl_texture_cube_map_seamless,
        gl_program_point_size
    };
    void gl_enable(capability capability);

    enum class depth_func : std::uint32_t {
        gl_never,
        gl_less,
        gl_equal,
        gl_lequal,
        gl_greater,
        gl_notequal,
        gl_gequal,
        gl_always
    };
    void gl_depth_func(depth_func depth_func);

    void init(int major, int minor);
    void terminate();

    template<std::floating_point T>
    T get_time();

    enum class name : std::uint32_t {
        gl_vendor,
        gl_renderer,
        gl_version,
        gl_shading_language_version
    };
    const char* get_string(name name);

    void check_opengl_errors();
}

template<>
    struct enable_bitmask_operators<tinygl::buffer_bit> {
    static constexpr bool enable = true;
};

extern template float tinygl::get_time<float>();
extern template double tinygl::get_time<double>();
extern template long double tinygl::get_time<long double>();

#endif // TINYGL_TINYGL_H
