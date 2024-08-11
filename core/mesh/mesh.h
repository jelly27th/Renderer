#ifndef _MESH_H
#define _MESH_H

#include "../common.h"
#include "../maths/maths.h"
#include "../darray/darray.h"
#include "../utils/utils.h"

typedef struct {
  vec4f vertex[3]; 
  vec3f uv[3];
  vec3f normal[3];
}triangle_t;

typedef struct {
    int triangles_Number;
    triangle_t* triangles;
}mesh_t;

mesh_t* mesh_load(const char* filename);
void mesh_free(mesh_t* mesh);

#endif