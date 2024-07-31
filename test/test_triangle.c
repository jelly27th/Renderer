#include <stdlib.h>

#include "../core/api.h"

int main(int argc, char** argv) {
  int width = 800, height = 800;
  image_t* image = image_new(width, height, 3);
  RGB color = {0, 0, 0};

  mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");

  image_t* _texture = image_load("../assets/african_head/african_head_diffuse.tga");
  texture_t* texture = image_to_texture(_texture);
  image_free(_texture);

  float aspect = 1;
  mat4f model = mat4_mult_mat4(mat4_mult_mat4(mat4_rotate_y(0.0f), mat4_scale(10,10,10)), mat4_translate(0,0,0));
  mat4f view = mat4_look_at(vec3_new(0, 0, 20), vec3_new(0, 0, 0), vec3_new(0, 1, 0));
  mat4f projection = mat4_perspective(90.0f, aspect, 0.1f, 40.0f);
  mat4f mvp = mat4_mult_mat4(projection, mat4_mult_mat4(view, model));

  vec3f light = vec3_new(0, 0, -1);

  static int idx = 0;
  for (int i = 0; i < mesh->v_indices_size; i++) {
    int v_indices[3] = {mesh->v_indices[i].x - 1, mesh->v_indices[i].y - 1,
                        mesh->v_indices[i].z - 1};
    int vt_indices[3] = {mesh->vt_indices[i].x - 1, mesh->vt_indices[i].y - 1,
                        mesh->vt_indices[i].z - 1};
    
    // vertex coordinates
    vec4f v[3];
    for (int i = 0; i < 3; i++) {
        v[i] = mat4_mult_vec4(mvp, vec3_2_vec4(mesh->v[v_indices[i]], 1));
    }

    // texture coordinates
    vec3f vt[3];
    for (int i = 0; i < 3; i++) {
        vt[i] = mesh->vt[vt_indices[i]];
    }

    vec3f _v[3];
    for (int i = 0; i < 3; i++) {
      _v[i] = mesh->v[v_indices[i]];
    }
    //        --->     --->
    // vector v0v1 and v0v2 can't switch positions
    vec3f normal = vec3_cross(vec3_sub(_v[2], _v[0]), vec3_sub(_v[1], _v[0]));
    vec3f normalize = vec3_normalize(normal);

    float intensity = vec3_dot(light, normalize);
    if (intensity > 0) {
        draw2d_triangle_raster(v, vt, texture, image, color, intensity);
    }
  }
  image_save("output.tga", image);
  texture_free(texture);
  image_free(image);

  return 0;
}