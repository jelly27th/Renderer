#include "../core/api.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  
  int width = 1000, height = 1000;
  int ndc_width = -1, ndc_height = 1;
  float aspect = 1;
  image_t* image = image_new(width, height, 3);
  RGB red = {255, 0, 0};
  // mesh_t* mesh = mesh_load("../assets/test/teapot.obj");
  mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");

   /*teapot*/
  // mat4f model = mat4_mult_mat4(mat4_mult_mat4(mat4_rotate_y(0.0f), mat4_scale(1,1,1)), mat4_translate(0,-5,0));
  // mat4f view = mat4_look_at(vec3_new(0, 0, 20), vec3_new(0, 0, 0), vec3_new(0, 1, 0));
  // mat4f projection = mat4_perspective(90.0f, aspect, 0.1f, 40.0f);

  mat4f model = mat4_mult_mat4(mat4_mult_mat4(mat4_rotate_y(0.0f), mat4_scale(10,10,10)), mat4_translate(0,0,0));
  mat4f view = mat4_look_at(vec3_new(0, 0, 20), vec3_new(0, 0, 0), vec3_new(0, 1, 0));
  mat4f projection = mat4_perspective(90.0f, aspect, 0.1f, 40.0f);
  mat4f mvp = mat4_mult_mat4(projection, mat4_mult_mat4(view, model));

  for (int i = 0; i < mesh->v_indices_size; i++) {
      
      int v_indices[3] = {mesh->v_indices[i].x - 1, 
                          mesh->v_indices[i].y - 1, 
                          mesh->v_indices[i].z - 1};
      for (int j = 0; j < 3; j++) {
      vec3f v0 = mesh->v[v_indices[j]];
      vec3f v1 = mesh->v[v_indices[(j + 1) % 3]];

      vec4f _v0 = mat4_mult_vec4(mvp, vec3_2_vec4(v0, 1));
      vec4f _v1 = mat4_mult_vec4(mvp, vec3_2_vec4(v1, 1));

      vec3f ndc_v0 = vec4_2_vec3(_v0, _v0.w);
      vec3f ndc_v1 = vec4_2_vec3(_v1, _v1.w);

      vec2i point0 = vec2_new(mapping_interval(ndc_width, ndc_height, 1, width, ndc_v0.x),
                              mapping_interval(ndc_width, ndc_height, 1, width, ndc_v0.y));
      vec2i point1 = vec2_new(mapping_interval(ndc_width, ndc_height, 1, width, ndc_v1.x),
                              mapping_interval(ndc_width, ndc_height, 1, width, ndc_v1.y));
      draw2d_line(point0, point1, red, image);
      } 
  }

  image_save("output.tga", image);
  image_free(image);

  return 0;
}