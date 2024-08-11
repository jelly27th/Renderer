#include "flat_shader.h"

vec4f flat_vertex_shader(void* _attr, void* _varyings, void* _uniforms) {
    
    flat_attribute_t *attribute = (flat_attribute_t *)_attr;
    flat_uniforms_t *uniforms = (flat_uniforms_t*)_uniforms;
    flat_varyings_t *varyings = (flat_varyings_t*)_varyings;

    vec4f local_coord = attribute->worldcoord;
    mat4f model_matrix = uniforms->model_matrix;
    mat4f view_matrix = uniforms->view_matrix;
    mat4f prespective_matrix = uniforms->perspective_matrix;
    mat4f mvp = mat4_mult_mat4(prespective_matrix, mat4_mult_mat4(view_matrix, model_matrix));
    vec4f clipcoord = mat4_mult_vec4(mvp, local_coord);

    varyings->texcoord = attribute->texcoord;

    return clipcoord;
}

vec4f flat_fragment_shader(void* _varyings, void* _uniforms, texture_t* texture) {

    flat_varyings_t* varyings = (flat_varyings_t*)_varyings;
    flat_uniforms_t* uniforms = (flat_uniforms_t*)_uniforms;

    vec2i texCoord = vec3_2_vec2(convert_texture_point(varyings->texcoord, texture));
    RGB _color = {255, 255, 255};
    get_texture_pixel(texture, _color, texCoord);
    vec3f normal = vec3_cross(vec3_sub(uniforms->worldcoord[2], uniforms->worldcoord[0]), 
                              vec3_sub(uniforms->worldcoord[1], uniforms->worldcoord[0]));
    vec3f normalize = vec3_normalize(normal);
    float instensity = vec3_dot(uniforms->light_dir, normalize);
    if (instensity > 0) {
        calculate_lighting_color(texture, _color, instensity);
    }
    vec4f color = vec4_new(_color[0], _color[1], _color[2], 0);
    
    return color;
}