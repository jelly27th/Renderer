#ifndef _MESH_H
#define _MESH_H

#include "../common.h"
#include "../maths/maths.h"
#include "../darray/darray.h"
#include "../utils/utils.h"

typedef struct {
    vec3f* v;
    int v_size;

    vec3f* vt;
    int vt_size;
    
    vec3f* vn;
    int vn_size;
    
    vec3i* v_indices;
    int v_indices_size;

    vec3i* vn_indices;
    int vn_indices_size;
    
    vec3i* vt_indices;
    int vt_indices_size;

}mesh_t;

mesh_t* mesh_load(const char* filename);
void mesh_free(mesh_t* mesh);

#endif