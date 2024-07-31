#include "../core/api.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  
  int width = 800, height = 800;
  int ndc_width = -1, ndc_height = 1;
  float aspect = 1;
  image_t* image = image_new(width, height, 3);
  RGB red = {255, 0, 0};
  mesh_t* mesh = mesh_load("../assets/test/teapot.obj");
  // mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");

  mat4f model = mat4_mult_mat4(mat4_mult_mat4(mat4_rotate_y(0.0f), mat4_scale(1,1,1)), mat4_translate(0,-5,0));
  // mat4f view = mat4_identity();
  mat4f view = mat4_look_at(vec3_new(0, 0, -20), vec3_new(0, 0, 0), vec3_new(0, 1, 0));
  mat4f projection = mat4_perspective(90.0f, aspect, 0.1f, 40.0f);
  // mat4f mvp = mat4_mult_mat4(projection, mat4_mult_mat4(view, model));
  mat4f mv = mat4_mult_mat4(view, model);

  FILE *opf = fopen("log.txt" , "w+");
  char origin[] = "origin: ";
  char camera[] = "camera: ";
  char project[] = "project: ";
  char final[] = "final: ";

  for (int i = 0; i < mesh->v_size; i++) {
      
      vec3f v0 = mesh->v[i];
      // printf("orign:"); vec3_print(v0);
      fputs(origin, opf); vec3_fprint(opf, v0);

      vec4f _v0 = mat4_mult_vec4(mv, vec3_2_vec4(v0, 1));
      // printf("camera:"); vec4_print(_v0);
      fputs(camera, opf); vec4_fprint(opf, _v0);

      vec4f p_v0 = mat4_mult_vec4(projection, _v0);
      // printf("project:"); vec4_print(p_v0);
      fputs(project, opf); vec4_fprint(opf, p_v0);

      vec3f ndc_v0 = vec4_2_vec3(p_v0, p_v0.w);
      printf("final:"); vec3_print(ndc_v0);
      fputs(final, opf); vec3_fprint(opf, ndc_v0);

        vec2i point0 = vec2_new(mapping_interval(ndc_width, ndc_height, 1, width, ndc_v0.x),
                              mapping_interval(ndc_width, ndc_height, 1, width, ndc_v0.y));
        draw2d_point(point0, red, image);
  }

  fclose(opf);
  image_save("output.tga", image);
  image_free(image);

  return 0;
}