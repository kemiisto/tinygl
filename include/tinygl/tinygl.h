#ifndef TINYGL_TINYGL_H
#define TINYGL_TINYGL_H

#include "tinygl/buffer.h"
#include "tinygl/shader.h"
#include "tinygl/shader_program.h"
#include "tinygl/texture.h"
#include "tinygl/vertex_array_object.h"
#include "tinygl/window.h"
#include "tinygl/keyboard.h"

namespace tinygl
{
    float getTime();
    const char* getString(GLenum name);
}

#endif // TINYGL_TINYGL_H
