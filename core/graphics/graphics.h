#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "../common.h"
#include "../image/image.h"
#include "../maths/maths.h"
#include "../utils/utils.h"
#include "../texture/texture.h"
#include "../../shader/shader.h"
#include "../draw2d/draw2d.h"

typedef vec4f (vertex_shader_t)(void* attribute, void* varying, void* uniform);
typedef vec4f (fragment_shader_t)(void* varying, void* uniform, texture_t* texture);

typedef struct {
    // shader
    void* attribute[3];
    void* varying;
    void* uniform;
    int varingSize;
    vertex_shader_t* vertex_shader;
    fragment_shader_t* fragment_shader;

    vec3f screen_coord[3];
    void* varying_buffer[3];
    vec3f zValue;

}program_t;

program_t* program_new(vertex_shader_t* vertex_shader, fragment_shader_t* fragment_shader,
                       int attributeSize, int varyingSize, int uniformSize);
void program_free(program_t* program);
void graphics_triangle_raster(program_t* program, texture_t* texture, image_t *image);

#endif