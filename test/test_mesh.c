#include "../core/api.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  
  int width = 1000, height = 1000;
  image_t* image = image_new(width, height, 3);
  RGB red = {255, 0, 0};

  mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");
  // MESH* mesh = mesh_load("../assets/test/triangle.obj");


  for (int i = 0; i < mesh->v_indices_size; i++) {
    
    printf("%d\n", i+3864);
    int v_indices[3] = {mesh->v_indices[i].x - 1, 
                        mesh->v_indices[i].y - 1, 
                        mesh->v_indices[i].z - 1};
    for (int j = 0; j < 3; j++) {
      vec3f v0 = mesh->v[v_indices[j]];
      vec3f v1 = mesh->v[v_indices[(j + 1) % 3]];
      vec2i point0 = vec2i_new(mapping_interval(-1, 1, 1, width, v0.x),
                                  mapping_interval(-1, 1, 1, width, v0.y));
      vec2i point1 = vec2i_new(mapping_interval(-1, 1, 1, width, v1.x),
                                  mapping_interval(-1, 1, 1, width, v1.y));
      draw2d_line(point0, point1, red, image);
    } 
  }

  image_save("output.tga", image);
  image_free(image);

  return 0;
}