#include "../core/api.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  
  int width = 1000, height = 1000;
  IMAGE* image = image_init(width, height, 3);
  RGB red = {255, 0, 0};

  MESH* mesh = mesh_load("../assets/african_head/african_head.obj");
  // MESH* mesh = mesh_load("../assets/test/triangle.obj");


  for (int i = 0; i < mesh->v_indices_size; i++) {
    
    printf("%d\n", i+3864);
    int v_indices[3] = {mesh->v_indices[i].x - 1, 
                        mesh->v_indices[i].y - 1, 
                        mesh->v_indices[i].z - 1};
    for (int j = 0; j < 3; j++) {
      vec3_f v0 = mesh->v[v_indices[j]];
      vec3_f v1 = mesh->v[v_indices[(j + 1) % 3]];
      vec2_i point0 = point_create(((v0.x + 1.0) * width / 2.0), 
                                   ((v0.y + 1.0) * height / 2.0));
      vec2_i point1 = point_create(((v1.x + 1.0) * width / 2.0),
                                   ((v1.y + 1.0) * height / 2.0));
      draw2d_line(point0, point1, red, image);
    } 
  }

  image_save("output.tga", image);
  image_free(image);

  return 0;
}