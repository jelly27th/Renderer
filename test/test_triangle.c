#include <stdlib.h>

#include "../core/api.h"

int main(int argc, char** argv) {
  int width = 800, height = 800;
  IMAGE* image = image_init(width, height, 3);
  RGB red = {255, 0, 0};

    /* test singal triangle */
  // vec2i v0 = vec2i_new(13, 178);
  // vec2i v1 = vec2i_new(17, 59);
  // vec2i v2 = vec2i_new(191, 2);
  // draw2d_triangle_raster(v0, v1, v2, red, image);

  MESH* mesh = mesh_load("../assets/african_head/african_head.obj");
  vec3f light = vec3f_new(0, 0, -1);

  for (int i = 0; i < mesh->v_indices_size; i++) {
    // printf("%d\n", i + 3864);
    int v_indices[3] = {mesh->v_indices[i].x - 1, mesh->v_indices[i].y - 1,
                        mesh->v_indices[i].z - 1};

    vec3f v0 = mesh->v[v_indices[0]];
    vec3f v1 = mesh->v[v_indices[1]];
    vec3f v2 = mesh->v[v_indices[2]];
    vec2i point0 = vec2i_new(mapping_interval(-1, 1, 1, width, v0.x),
                                mapping_interval(-1, 1, 1, width, v0.y));
    vec2i point1 = vec2i_new(mapping_interval(-1, 1, 1, width, v1.x),
                                mapping_interval(-1, 1, 1, width, v1.y));
    vec2i point2 = vec2i_new(mapping_interval(-1, 1, 1, width, v2.x),
                               mapping_interval(-1, 1, 1, width, v2.y));
    //        --->     --->
    // vector v0v1 and v0v2 can't switch positions
    vec3f normal = vec3f_cross(vec3f_sub(v2, v0), vec3f_sub(v1, v0));
    vec3f normalize = vec3f_normalize(normal);
    // normalize.x = -normalize.x; normalize.y = -normalize.y; normalize.z = -normalize.z;
    printf("%f %f %f\n", normalize.x, normalize.y, normalize.z);
    float intensity = vec3f_dot(light, normalize);
    red[0] = intensity * 255;
    red[1] = intensity * 255;
    red[2] = intensity * 255;
    if (intensity > 0) {
        draw2d_triangle_raster(point0, point1, point2, red, image);
    } 
  }

  image_save("output.tga", image);
  image_free(image);

  return 0;
}