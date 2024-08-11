#include "gouraud_shader.h"

vec4f gouraud_vertex_shader(void* _attr, void* _varyings, void* _uniforms) {
    
    gouraud_attribute_t *attribute = (gouraud_attribute_t *)_attr;
    gouraud_uniforms_t *uniforms = (gouraud_uniforms_t*)_uniforms;
    gouraud_varyings_t *varyings = (gouraud_varyings_t*)_varyings;

    vec4f local_coord = attribute->worldcoord;
    mat4f model_matrix = uniforms->model_matrix;
    mat4f view_matrix = uniforms->view_matrix;
    mat4f prespective_matrix = uniforms->perspective_matrix;
    mat4f mvp = mat4_mult_mat4(prespective_matrix, mat4_mult_mat4(view_matrix, model_matrix));
    vec4f clipcoord = mat4_mult_vec4(mvp, local_coord);

    varyings->texcoord = attribute->texcoord;

    varyings->instensity = vec3_dot(attribute->normal, uniforms->light_dir);

    return clipcoord;
}

vec4f gouraud_fragment_shader(void* _varyings, void* _uniforms, texture_t* texture) {

    gouraud_varyings_t* varyings = (gouraud_varyings_t*)_varyings;
    gouraud_uniforms_t* uniforms = (gouraud_uniforms_t*)_uniforms;

    vec2i texCoord = vec3_2_vec2(convert_texture_point(varyings->texcoord, texture));
    RGB _color = {255, 255, 255};
    get_texture_pixel(texture, _color, texCoord);

    float instensity = varyings->instensity;
    if (instensity > 0) {
        calculate_lighting_color(texture, _color, instensity);
    } else {
        calculate_lighting_color(texture, _color, -instensity);
    }
    vec4f color = vec4_new(_color[0], _color[1], _color[2], 0);
    
    return color;
}