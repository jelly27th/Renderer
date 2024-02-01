#ifndef _MESH_H
#define _MESH_H

#include "../common.h"
#include "../maths/maths.h"
#include "../darray/darray.h"
#include "../utils/utils.h"

typedef struct {
    vec3_f* v;
    int v_size;

    vec3_f* vt;
    int vt_size;
    
    vec3_f* vn;
    int vn_size;
    
    vec3_i* v_indices;
    int v_indices_size;

    vec3_i* vn_indices;
    int vn_indices_size;
    
    vec3_i* vt_indices;
    int vt_indices_size;

}MESH;

MESH* mesh_load(const char* filename);
void mesh_free(MESH* mesh);

#endif