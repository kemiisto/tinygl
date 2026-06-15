#ifndef TINYGL_TEXTURE_H
#define TINYGL_TEXTURE_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

namespace tinygl
{
    class texture final
    {
    public:
        enum class target : uint32_t {
            gl_target_1d,
            gl_target_2d,
            gl_target_3d,
            gl_target_1d_array,
            gl_target_2d_array,
            gl_target_rectangle,
            gl_target_cube_map,
            gl_target_cube_map_array,
            gl_target_buffer,
            gl_target_2d_multisample,
            gl_target_2d_multisample_array
        };

        enum class coordinate : uint32_t {
            s,
            t,
            r
        };

        enum class wrap_mode : uint32_t {
            gl_clamp_to_edge,
            gl_clamp_to_border,
            gl_mirrored_repeat,
            gl_repeat,
            gl_mirror_clamp_to_edge
        };

        enum class filter : uint32_t {
            nearest,
            linear,
            nearest_mip_map_nearest,
            nearest_mip_map_linear,
            linear_mip_map_nearest,
            linear_mip_map_linear
        };

        enum class internal_format : uint32_t {
            // base internal formats
            gl_depth_component,
            gl_depth_stencil,
            gl_red,
            gl_rg,
            gl_rgb,
            gl_rgba,
            // sized internal format
            gl_r8,
            gl_r8_snorm,
            gl_r16,
            gl_r16_snorm,
            gl_rg8,
            gl_rg8_snorm,
            gl_rg16,
            gl_rg16_snorm,
            gl_r3_g3_b2,
            gl_rgb4,
            gl_rgb5,
            gl_rgb8,
            gl_rgb8_snorm,
            gl_rgb10,
            gl_rgb12,
            gl_rgb16_snorm,
            gl_rgba2,
            gl_rgba4,
            gl_rgb5_a1,
            gl_rgba8,
            gl_rgba8_snorm,
            gl_rgb10_a2,
            gl_rgb10_a2ui,
            gl_rgba12,
            gl_rgba16,
            gl_srgb8,
            gl_srgb8_alpha8,
            gl_r16f,
            gl_rg16f,
            gl_rgb16f,
            gl_rgba16f,
            gl_r32f,
            gl_rg32f,
            gl_rgb32f,
            gl_rgba32f,
            gl_r11f_g11f_b10f,
            gl_rgb9_e5,
            gl_r8i,
            gl_r8ui,
            gl_r16i,
            gl_r16ui,
            gl_r32i,
            gl_r32ui,
            gl_rg8i,
            gl_rg8ui,
            gl_rg16i,
            gl_rg16ui,
            gl_rg32i,
            gl_rg32ui,
            gl_rgb8i,
            gl_rgb8ui,
            gl_rgb16i,
            gl_rgb16ui,
            gl_rgb32i,
            gl_rgb32ui,
            gl_rgba8i,
            gl_rgba8ui,
            gl_rgba16i,
            gl_rgba16ui,
            gl_rgba32i,
            gl_rgba32ui,
            // compressed internal formats
            gl_compressed_red,
            gl_compressed_rg,
            gl_compressed_rgb,
            gl_compressed_rgba,
            gl_compressed_srgb,
            gl_compressed_srgb_alpha,
            gl_compressed_red_rgtc1,
            gl_compressed_signed_red_rgtc1,
            gl_compressed_rg_rgtc2,
            gl_compressed_signed_rg_rgtc2,
            gl_compressed_rgba_bptc_unorm,
            gl_compressed_srgb_alpha_bptc_unorm,
            gl_compressed_rgb_bptc_signed_float,
            gl_compressed_rgb_bptc_unsigned_float
        };

        enum class format : uint32_t {
            gl_red,
            gl_rg,
            gl_rgb,
            gl_bgr,
            gl_rgba,
            gl_bgra,
            gl_red_integer,
            gl_rg_integer,
            gl_rgb_integer,
            gl_bgr_integer,
            gl_rgba_integer,
            gl_bgra_integer,
            gl_stencil_index,
            gl_depth_component,
            gl_depth_stencil
        };

        texture(
            target target,
            const std::filesystem::path& file_name,
            internal_format internal_format,
            format format,
            uint32_t unit);
        ~texture();

        texture(texture&& other) noexcept;
        texture& operator=(texture&& other) noexcept;

        void bind();
        void unbind();

        void generate_mipmaps();

        void set_wrap_mode(wrap_mode mode);
        void set_wrap_mode(coordinate direction, wrap_mode mode);
        wrap_mode get_wrap_mode(coordinate direction) const;

        void set_minification_filter(filter filter);
        filter minification_filter() const;

        void set_magnification_filter(filter filter);
        filter magnification_filter() const;

        void set_min_mag_filters(filter minification_filter, filter magnification_filter);
        std::pair<filter, filter> min_mag_filters() const;

        static std::string to_string(const coordinate& direction);
        static std::string to_string(const target& target);

    private:
        struct texture_private;
        std::unique_ptr<texture_private> p;
    };
}

#endif // TINYGL_TEXTURE_H
