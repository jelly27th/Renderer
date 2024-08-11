#include <stdlib.h>

#include "../core/api.h"
#include "../shader/shader.h"

vec3f V_TMP[3] = {0};
int main(int argc, char** argv) {
  int width = 800, height = 800;
  image_t* image = image_new(width, height, 3);

  mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");

  image_t* _texture = image_load("../assets/african_head/african_head_diffuse.tga");
  texture_t* texture = image_to_texture(_texture);
  image_free(_texture);

  float aspect = 1;
  mat4f model = mat4_mult_mat4(mat4_mult_mat4(mat4_rotate_y(0.0f), mat4_scale(10,10,10)), mat4_translate(0,0,0));
  mat4f view = mat4_look_at(vec3_new(4, 0, 15), vec3_new(0, 0, 0), vec3_new(0, 1, 0));
  mat4f perspective = mat4_perspective(90.0f, aspect, 0.1f, 60.0f);
  vec3f light = vec3_new(0, 0, -1);

  program_t* program = program_new(flat_vertex_shader, flat_fragment_shader,
                                   sizeof(flat_attribute_t), sizeof(flat_varyings_t), sizeof(flat_uniforms_t));
  
  flat_uniforms_t* uniforms = (flat_uniforms_t*)program->uniform;
  uniforms->light_dir = light;
  uniforms->model_matrix = model;
  uniforms->view_matrix = view;
  uniforms->perspective_matrix = perspective;

  for (int i = 0; i < mesh->triangles_Number; i++) {
    
    // vertex coordinates and texture coordinates
    for (int j = 0; j < 3; j++) {
      flat_attribute_t* attritube = (flat_attribute_t*)program->attribute[j];
      V_TMP[j] = vec4_2_vec3(mesh->triangles[i].vertex[j], 1);
      attritube->worldcoord = mesh->triangles[i].vertex[j];
      attritube->texcoord = mesh->triangles[i].uv[j];
      uniforms->worldcoord[j] = vec4_2_vec3(attritube->worldcoord, 1);
    }

    graphics_triangle_raster(program, texture, image);
  }

  image_save("output.tga", image);
  texture_free(texture);
  image_free(image);
  mesh_free(mesh);

  return 0;
}