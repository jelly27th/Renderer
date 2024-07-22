#include <stdlib.h>

#include "../core/api.h"

int main(int argc, char** argv) {
  int width = 800, height = 800;
  image_t* image = image_new(width, height, 3);
  RGB color = {255, 0, 0};

  mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");

  image_t* _texture = image_load("../assets/african_head/african_head_diffuse.tga");
  texture_t* texture = image_to_texture(_texture);
  image_free(_texture);

  vec3f light = vec3_new(0, 0, -1);

  for (int i = 0; i < mesh->v_indices_size; i++) {
    int v_indices[3] = {mesh->v_indices[i].x - 1, mesh->v_indices[i].y - 1,
                        mesh->v_indices[i].z - 1};
    int vt_indices[3] = {mesh->vt_indices[i].x - 1, mesh->vt_indices[i].y - 1,
                        mesh->vt_indices[i].z - 1};
    
    // vertex coordinates
    vec3f v0 = mesh->v[v_indices[0]];
    vec3f v1 = mesh->v[v_indices[1]];
    vec3f v2 = mesh->v[v_indices[2]];

    // texture coordinates
    vec3f vt0 = mesh->vt[vt_indices[0]];
    vec3f vt1 = mesh->vt[vt_indices[1]];
    vec3f vt2 = mesh->vt[vt_indices[2]];

    //        --->     --->
    // vector v0v1 and v0v2 can't switch positions
    vec3f normal = vec3_cross(vec3_sub(v2, v0), vec3_sub(v1, v0));
    vec3f normalize = vec3_normalize(normal);

    float intensity = vec3_dot(light, normalize);
    if (intensity > 0) {
        draw2d_triangle_raster(v0, v1, v2, vt0, vt1, vt2, 
                               texture, image, color, intensity);
    }
  }
  image_save("output.tga", image);
  texture_free(texture);
  image_free(image);

  return 0;
}