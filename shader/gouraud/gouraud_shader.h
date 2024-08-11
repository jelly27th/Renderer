#ifndef _GOURAUD_SHADER_H
#define _GOURAUD_SHADER_H

#include "../../core/common.h"
#include "../../core/utils/utils.h"
#include "../../core/image/image.h"
#include "../../core/maths/maths.h"
#include "../../core/texture/texture.h"

typedef struct {
  vec4f worldcoord;
  vec3f texcoord;
  vec3f normal;
}gouraud_attribute_t;

typedef struct {
  vec3f texcoord;
  float instensity;
}gouraud_varyings_t;

typedef struct {
  mat4f model_matrix;
  mat4f view_matrix;
  mat4f perspective_matrix;
  vec3f light_dir;
}gouraud_uniforms_t;

vec4f gouraud_vertex_shader(void* _attr, void* _varyings, void* _uniforms);
vec4f gouraud_fragment_shader(void* _varyings, void* _uniforms, texture_t* texture);

#endif