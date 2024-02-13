#include "mesh.h"

static mesh_t* obj_load(const char* filename);
static mesh_t* mesh_struct_new();

/**
 * @brief load obj file and return mesh object
 * 
 * @param filename 
 * @return MESH* 
 * @file mesh.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-20
 * @copyright Copyright (c) 2024
 */
mesh_t* mesh_load(const char* filename) {
    return obj_load(filename);
}

static mesh_t* obj_load(const char* filename) {
    
    char line[256]; // buffer size

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
      error_check(filename, errno);
    }
    mesh_t* mesh = mesh_struct_new();
    while(!feof(fp)) {
        
        fgets(line, 256, fp); // read line

        if (0 == strncmp(line, "v ", 2)) {
            vec3f v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            darray_push(mesh->v, v);
            mesh->v_size++;
        } else if (0 == strncmp(line, "vt", 2)) {
            vec3f vt;
            sscanf(line, "vt %f %f %f", &vt.x, &vt.y, &vt.z);
            darray_push(mesh->vt, vt);
            mesh->vt_size++;
        } else if (0 == strncmp(line, "vn", 2)) {
            vec3f vn;
            sscanf(line, "vn %f %f %f", &vn.x, &vn.y, &vn.z);
            darray_push(mesh->vn, vn);
            mesh->vn_size++;
        } else if (0 == strncmp(line, "f ", 2)) {
            vec3i v_indices, vt_indices, vn_indices;
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &v_indices.x, &vt_indices.x, &vn_indices.x,
                   &v_indices.y, &vt_indices.y, &vn_indices.y,
                   &v_indices.z, &vt_indices.z, &vn_indices.z);
            darray_push(mesh->v_indices, v_indices);
            darray_push(mesh->vt_indices, vt_indices);
            darray_push(mesh->vn_indices, vn_indices);
            mesh->v_indices_size++;
            mesh->vt_indices_size++;
            mesh->vn_indices_size++;
        }
    };
    
    fclose(fp);

    return mesh;
}

static mesh_t* mesh_struct_new() {
    mesh_t* mesh = malloc(sizeof(mesh_t));
    mesh->v = NULL;
    mesh->v_size = 0;
    mesh->vt = NULL;
    mesh->vt_size = 0;
    mesh->vn = NULL;
    mesh->vn_size = 0;
    mesh->v_indices = NULL;
    mesh->v_indices_size = 0;
    mesh->vn_indices = NULL;
    mesh->vn_indices_size = 0;
    mesh->vt_indices = NULL;
    mesh->vt_indices_size = 0;

    return mesh;
}

void mesh_free(mesh_t* mesh) {

    free(mesh->v);
    free(mesh->vt);
    free(mesh->vn);
    free(mesh->v_indices);
    free(mesh->vn_indices);
    free(mesh->vt_indices);
    free(mesh);
}
