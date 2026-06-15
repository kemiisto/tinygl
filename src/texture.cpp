#include "tinygl/exceptions.h"
#include "tinygl/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <GL/glew.h>
#include <map>
#include <stdexcept>

namespace {
    constexpr GLenum gl_enum(tinygl::texture::target target)
    {
        switch(target) {
        case tinygl::texture::target::target_1d: return GL_TEXTURE_BINDING_1D;
        case tinygl::texture::target::target_2d: return GL_TEXTURE_BINDING_2D;
        case tinygl::texture::target::target_3d: return GL_TEXTURE_BINDING_3D;
        case tinygl::texture::target::target_1d_array: return GL_TEXTURE_BINDING_1D_ARRAY;
        case tinygl::texture::target::target_2d_array: return GL_TEXTURE_BINDING_2D_ARRAY;
        case tinygl::texture::target::target_rectangle: return GL_TEXTURE_BINDING_RECTANGLE;
        case tinygl::texture::target::target_cube_map: return GL_TEXTURE_BINDING_CUBE_MAP;
        case tinygl::texture::target::target_cube_map_array: return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
        case tinygl::texture::target::target_buffer: return GL_TEXTURE_BINDING_BUFFER;
        case tinygl::texture::target::target_2d_multisample: return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
        case tinygl::texture::target::target_2d_multisample_array: return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
        }
    }

    constexpr GLenum gl_enum(tinygl::texture::coordinate_direction coordinate_direction)
    {
        switch(coordinate_direction) {
        case tinygl::texture::coordinate_direction::s: return GL_TEXTURE_WRAP_S;
        case tinygl::texture::coordinate_direction::t: return GL_TEXTURE_WRAP_T;
        case tinygl::texture::coordinate_direction::r: return GL_TEXTURE_WRAP_R;
        }
    }

    constexpr GLint gl_int(tinygl::texture::wrap_mode wrap_mode)
    {
        switch(wrap_mode) {
        case tinygl::texture::wrap_mode::repeat: return GL_REPEAT;
        case tinygl::texture::wrap_mode::mirrored_repeat: return GL_MIRRORED_REPEAT;
        case tinygl::texture::wrap_mode::clamp_to_edge: return GL_CLAMP_TO_EDGE;
        case tinygl::texture::wrap_mode::clamp_to_border: return GL_CLAMP_TO_BORDER;
        }
    }

    constexpr GLint gl_int(tinygl::texture::filter filter)
    {
        switch(filter) {
        case tinygl::texture::filter::nearest: return GL_NEAREST;
        case tinygl::texture::filter::linear: return GL_LINEAR;
        case tinygl::texture::filter::nearest_mip_map_nearest: return GL_NEAREST_MIPMAP_NEAREST;
        case tinygl::texture::filter::nearest_mip_map_linear: return GL_NEAREST_MIPMAP_LINEAR;
        case tinygl::texture::filter::linear_mip_map_nearest: return GL_LINEAR_MIPMAP_NEAREST;
        case tinygl::texture::filter::linear_mip_map_linear: return GL_LINEAR_MIPMAP_LINEAR;
        }
    }

    constexpr GLint gl_int(tinygl::texture::internal_format internal_format) {
        switch(internal_format) {
        case tinygl::texture::internal_format::gl_depth_component: return GL_DEPTH_COMPONENT;
        case tinygl::texture::internal_format::gl_depth_stencil: return GL_DEPTH_STENCIL;
        case tinygl::texture::internal_format::gl_red: return GL_RED;
        case tinygl::texture::internal_format::gl_rg: return GL_RG;
        case tinygl::texture::internal_format::gl_rgb: return GL_RGB;
        case tinygl::texture::internal_format::gl_rgba: return GL_RGBA;
        case tinygl::texture::internal_format::gl_r8: return GL_R8;
        case tinygl::texture::internal_format::gl_r8_snorm: return GL_R8_SNORM;
        case tinygl::texture::internal_format::gl_r16: return GL_R16;
        case tinygl::texture::internal_format::gl_r16_snorm: return GL_R16_SNORM;
        case tinygl::texture::internal_format::gl_rg8: return GL_RG8;
        case tinygl::texture::internal_format::gl_rg8_snorm: return GL_RG8_SNORM;
        case tinygl::texture::internal_format::gl_rg16: return GL_RG16;
        case tinygl::texture::internal_format::gl_rg16_snorm: return GL_RG16_SNORM;
        case tinygl::texture::internal_format::gl_r3_g3_b2: return GL_R3_G3_B2;
        case tinygl::texture::internal_format::gl_rgb4: return GL_RGB4;
        case tinygl::texture::internal_format::gl_rgb5: return GL_RGB5;
        case tinygl::texture::internal_format::gl_rgb8: return GL_RGB8;
        case tinygl::texture::internal_format::gl_rgb8_snorm: return GL_RGB8_SNORM;
        case tinygl::texture::internal_format::gl_rgb10: return GL_RGB10;
        case tinygl::texture::internal_format::gl_rgb12: return GL_RGB12;
        case tinygl::texture::internal_format::gl_rgb16_snorm: return GL_RGB16_SNORM;
        case tinygl::texture::internal_format::gl_rgba2: return GL_RGBA2;
        case tinygl::texture::internal_format::gl_rgba4: return GL_RGBA4;
        case tinygl::texture::internal_format::gl_rgb5_a1: return GL_RGB5_A1;
        case tinygl::texture::internal_format::gl_rgba8: return GL_RGBA8;
        case tinygl::texture::internal_format::gl_rgba8_snorm: return GL_RGBA8_SNORM;
        case tinygl::texture::internal_format::gl_rgb10_a2: return GL_RGB10_A2;
        case tinygl::texture::internal_format::gl_rgb10_a2ui: return GL_RGB10_A2UI;
        case tinygl::texture::internal_format::gl_rgba12: return GL_RGBA12;
        case tinygl::texture::internal_format::gl_rgba16: return GL_RGBA16;
        case tinygl::texture::internal_format::gl_srgb8: return GL_SRGB8;
        case tinygl::texture::internal_format::gl_srgb8_alpha8: return GL_SRGB8_ALPHA8;
        case tinygl::texture::internal_format::gl_r16f: return GL_R16F;
        case tinygl::texture::internal_format::gl_rg16f: return GL_RG16F;
        case tinygl::texture::internal_format::gl_rgb16f: return GL_RGB16F;
        case tinygl::texture::internal_format::gl_rgba16f: return GL_RGBA16F;
        case tinygl::texture::internal_format::gl_r32f: return GL_R32F;
        case tinygl::texture::internal_format::gl_rg32f: return GL_RG32F;
        case tinygl::texture::internal_format::gl_rgb32f: return GL_RGB32F;
        case tinygl::texture::internal_format::gl_rgba32f: return GL_RGBA32F;
        case tinygl::texture::internal_format::gl_r11f_g11f_b10f: return GL_R11F_G11F_B10F;
        case tinygl::texture::internal_format::gl_rgb9_e5: return GL_RGB9_E5;
        case tinygl::texture::internal_format::gl_r8i: return GL_R8I;
        case tinygl::texture::internal_format::gl_r8ui: return GL_R8UI;
        case tinygl::texture::internal_format::gl_r16i: return GL_R16I;
        case tinygl::texture::internal_format::gl_r16ui: return GL_R16UI;
        case tinygl::texture::internal_format::gl_r32i: return GL_R32I;
        case tinygl::texture::internal_format::gl_r32ui: return GL_R32UI;
        case tinygl::texture::internal_format::gl_rg8i: return GL_RG8I;
        case tinygl::texture::internal_format::gl_rg8ui: return GL_RG8UI;
        case tinygl::texture::internal_format::gl_rg16i: return GL_RG16I;
        case tinygl::texture::internal_format::gl_rg16ui: return GL_RG16UI;
        case tinygl::texture::internal_format::gl_rg32i: return GL_RG32I;
        case tinygl::texture::internal_format::gl_rg32ui: return GL_RG32UI;
        case tinygl::texture::internal_format::gl_rgb8i: return GL_RGB8I;
        case tinygl::texture::internal_format::gl_rgb8ui: return GL_RGB8UI;
        case tinygl::texture::internal_format::gl_rgb16i: return GL_RGB16I;
        case tinygl::texture::internal_format::gl_rgb16ui: return GL_RGB16UI;
        case tinygl::texture::internal_format::gl_rgb32i: return GL_RGB32I;
        case tinygl::texture::internal_format::gl_rgb32ui: return GL_RGB32UI;
        case tinygl::texture::internal_format::gl_rgba8i: return GL_RGBA8I;
        case tinygl::texture::internal_format::gl_rgba8ui: return GL_RGBA8UI;
        case tinygl::texture::internal_format::gl_rgba16i: return GL_RGBA16I;
        case tinygl::texture::internal_format::gl_rgba16ui: return GL_RGBA16UI;
        case tinygl::texture::internal_format::gl_rgba32i: return GL_RGBA32I;
        case tinygl::texture::internal_format::gl_rgba32ui: return GL_RGBA32UI;
        case tinygl::texture::internal_format::gl_compressed_red: return GL_COMPRESSED_RED;
        case tinygl::texture::internal_format::gl_compressed_rg: return GL_COMPRESSED_RG;
        case tinygl::texture::internal_format::gl_compressed_rgb: return GL_COMPRESSED_RGB;
        case tinygl::texture::internal_format::gl_compressed_rgba: return GL_COMPRESSED_RGBA;
        case tinygl::texture::internal_format::gl_compressed_srgb: return GL_COMPRESSED_SRGB;
        case tinygl::texture::internal_format::gl_compressed_srgb_alpha: return GL_COMPRESSED_SRGB_ALPHA;
        case tinygl::texture::internal_format::gl_compressed_red_rgtc1: return GL_COMPRESSED_RED_RGTC1;
        case tinygl::texture::internal_format::gl_compressed_signed_red_rgtc1: return GL_COMPRESSED_SIGNED_RED_RGTC1;
        case tinygl::texture::internal_format::gl_compressed_rg_rgtc2: return GL_COMPRESSED_RG_RGTC2;
        case tinygl::texture::internal_format::gl_compressed_signed_rg_rgtc2: return GL_COMPRESSED_SIGNED_RG_RGTC2;
        case tinygl::texture::internal_format::gl_compressed_rgba_bptc_unorm: return GL_COMPRESSED_RGBA_BPTC_UNORM;
        case tinygl::texture::internal_format::gl_compressed_srgb_alpha_bptc_unorm: return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
        case tinygl::texture::internal_format::gl_compressed_rgb_bptc_signed_float: return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        case tinygl::texture::internal_format::gl_compressed_rgb_bptc_unsigned_float: return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        }
    }

    constexpr GLenum gl_enum(tinygl::texture::format format)
    {
        switch(format) {
        case tinygl::texture::format::gl_red: return GL_RED;
        case tinygl::texture::format::gl_rg: return GL_RG;
        case tinygl::texture::format::gl_rgb: return GL_RGB;
        case tinygl::texture::format::gl_bgr: return GL_BGR;
        case tinygl::texture::format::gl_rgba: return GL_RGBA;
        case tinygl::texture::format::gl_bgra: return GL_BGRA;
        case tinygl::texture::format::gl_red_integer: return GL_RED_INTEGER;
        case tinygl::texture::format::gl_rg_integer: return GL_RG_INTEGER;
        case tinygl::texture::format::gl_rgb_integer: return GL_RGB_INTEGER;
        case tinygl::texture::format::gl_bgr_integer: return GL_BGR_INTEGER;
        case tinygl::texture::format::gl_rgba_integer: return GL_RGBA_INTEGER;
        case tinygl::texture::format::gl_bgra_integer: return GL_BGRA_INTEGER;
        case tinygl::texture::format::gl_stencil_index: return GL_STENCIL_INDEX;
        case tinygl::texture::format::gl_depth_component: return GL_DEPTH_COMPONENT;
        case tinygl::texture::format::gl_depth_stencil: return GL_DEPTH_STENCIL;
        }
    }
}

struct tinygl::texture::texture_private
{
    explicit texture_private(target t, GLuint u);
    ~texture_private() = default;

    bool bound();

    target texture_target;
    GLuint id = 0;
    GLuint unit = 0;

    std::map<texture::coordinate_direction, texture::wrap_mode> wrap_modes = {
        { texture::coordinate_direction::s, texture::wrap_mode::repeat },
        { texture::coordinate_direction::t, texture::wrap_mode::repeat },
        { texture::coordinate_direction::r, texture::wrap_mode::repeat }
    };

    texture::filter min_filter = filter::nearest_mip_map_linear;
    texture::filter mag_filter = filter::linear;
};

tinygl::texture::texture_private::texture_private(target t, GLuint u) :
        texture_target{t}, unit{u}
{
    glGenTextures(1, &id);
}

bool tinygl::texture::texture_private::bound()
{
    GLint bound_id;
    glGetIntegerv(gl_enum(texture_target), &bound_id);
    return id == static_cast<GLuint>(bound_id);
}

tinygl::texture::texture(target target,
                         const std::filesystem::path& file_name,
                         internal_format internal_format,
                         format format,
                         bool genMipMaps,
                         uint32_t unit) :
        p{std::make_unique<texture_private>(target, unit)}
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    auto* data = stbi_load(file_name.string().c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture!");
    }

    bind();

    switch (target) {
        case target::target_2d:
            glTexImage2D(GL_TEXTURE_2D, 0, gl_int(internal_format), width, height, 0, gl_enum(format), GL_UNSIGNED_BYTE, data);
            break;
        default:
            stbi_image_free(data);
            throw std::runtime_error("[tinygl::texture] texture target is not handled yet!");
    }

    if (genMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    unbind();
    stbi_image_free(data);
}

tinygl::texture::~texture() = default;

tinygl::texture::texture(tinygl::texture&& other) noexcept = default;

tinygl::texture& tinygl::texture::operator=(tinygl::texture&& other) noexcept
{
    if (this != &other) {
        p = std::move(other.p);
    }
    return *this;
}

void tinygl::texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + p->unit);
    glBindTexture(gl_enum(p->texture_target), p->id);
}

void tinygl::texture::unbind()
{
    glActiveTexture(GL_TEXTURE0 + p->unit);
    glBindTexture(gl_enum(p->texture_target), 0);
}

void tinygl::texture::set_wrap_mode(tinygl::texture::wrap_mode mode)
{
    assert(p->bound());

    switch (p->texture_target) {
        case target::target_1d:
        case target::target_1d_array:
        case target::target_buffer:
            p->wrap_modes.at(coordinate_direction::s) = mode;
            glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_WRAP_S, gl_int(mode));
            break;
        case target::target_2d:
        case target::target_2d_array:
        case target::target_cube_map:
        case target::target_cube_map_array:
        case target::target_2d_multisample:
        case target::target_2d_multisample_array:
        case target::target_rectangle:
            p->wrap_modes.at(coordinate_direction::s) = p->wrap_modes.at(coordinate_direction::t) = mode;
            glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_WRAP_S, gl_int(mode));
            glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_WRAP_T, gl_int(mode));
            break;
        case target::target_3d:
            p->wrap_modes.at(coordinate_direction::s) =
                p->wrap_modes.at(coordinate_direction::t) =
                    p->wrap_modes.at(coordinate_direction::r) = mode;
            glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_WRAP_S, gl_int(mode));
            glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_WRAP_T, gl_int(mode));
            glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_WRAP_R, gl_int(mode));
            break;
    }
}

void tinygl::texture::set_wrap_mode(
        tinygl::texture::coordinate_direction direction,
        tinygl::texture::wrap_mode mode)
{
    assert(p->bound());

    switch (p->texture_target) {
        case target::target_1d:
        case target::target_1d_array:
        case target::target_buffer:
            assert(direction == coordinate_direction::s);
            p->wrap_modes.at(direction) = mode;
            glTexParameteri(gl_enum(p->texture_target), gl_enum(direction), gl_int(mode));
            break;
        case texture::target::target_2d:
        case texture::target::target_2d_array:
        case texture::target::target_cube_map:
        case texture::target::target_cube_map_array:
        case texture::target::target_2d_multisample:
        case texture::target::target_2d_multisample_array:
        case texture::target::target_rectangle:
            assert(direction == coordinate_direction::s || direction == coordinate_direction::t);
            p->wrap_modes.at(direction) = mode;
            glTexParameteri(gl_enum(p->texture_target), gl_enum(direction), gl_int(mode));
            break;
        case target::target_3d:
            p->wrap_modes.at(direction) = mode;
            glTexParameteri(gl_enum(p->texture_target), gl_enum(direction), gl_int(mode));
            break;
    }
}

tinygl::texture::wrap_mode tinygl::texture::get_wrap_mode(tinygl::texture::coordinate_direction direction) const
{
    return p->wrap_modes.at(direction);
}

void tinygl::texture::set_minification_filter(tinygl::texture::filter filter)
{
    assert(p->bound());
    glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_MIN_FILTER, gl_int(filter));
    p->min_filter = filter;
}

tinygl::texture::filter tinygl::texture::minification_filter() const
{
    return p->min_filter;
}

void tinygl::texture::set_magnification_filter(tinygl::texture::filter filter)
{
    assert(p->bound());
    glTexParameteri(gl_enum(p->texture_target), GL_TEXTURE_MAG_FILTER, gl_int(filter));
    p->mag_filter = filter;
}

tinygl::texture::filter tinygl::texture::magnification_filter() const
{
    return p->mag_filter;
}

void tinygl::texture::set_min_mag_filters(
    tinygl::texture::filter minification_filter, tinygl::texture::filter magnification_filter)
{
    assert(p->bound());
    set_minification_filter(minification_filter);
    set_magnification_filter(magnification_filter);
}

std::string tinygl::texture::to_string(const tinygl::texture::coordinate_direction& direction)
{
    switch (direction) {
        case coordinate_direction::s: return "S";
        case coordinate_direction::t: return "T";
        case coordinate_direction::r: return "R";
        default: return "";
    }
}

std::string tinygl::texture::to_string(const tinygl::texture::target& target)
{
    switch (target) {
        case target::target_1d: return "1D";
        case target::target_2d: return "2D";
        case target::target_3d: return "3D";
        case target::target_1d_array: return "1DArray";
        case target::target_2d_array: return "2DArray";
        case target::target_rectangle: return "Rectangle";
        case target::target_cube_map: return "CubeMap";
        case target::target_cube_map_array: return "CubeMapArray";
        case target::target_buffer: return "Buffer";
        case target::target_2d_multisample: return "2DMultisample";
        case target::target_2d_multisample_array: return "2DMultisampleArray";
        default: return "";
    }
}
