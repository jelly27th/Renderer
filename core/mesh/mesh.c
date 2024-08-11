/******************************************************************************
 * @file mesh.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-04-13
 * @copyright Copyright (c) 2024
******************************************************************************/
#include "mesh.h"

static mesh_t* obj_load(const char* filename);
static mesh_t* mesh_new(vec3f* vertex_coord, vec3f* texture_coord, vec3f* normal, 
                        vec3i* vertex_indices, vec3i* texture_indices, vec3i* normal_indices);

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

/******************************************************************************
 * @brief 
 * 
 * @param filename 
 * @return mesh_t* 
 * @file mesh.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-04-13
 * @copyright Copyright (c) 2024
******************************************************************************/
static mesh_t* obj_load(const char* filename) {
    
    vec3f* vertex_coord = NULL;
    vec3f* texture_coord = NULL;
    vec3f* normal = NULL;
    vec3i* vertex_indices = NULL;
    vec3i* normal_indices = NULL;
    vec3i* texture_indices = NULL;

    char line[256]; // buffer size
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
      error_check(filename, errno);
    }

    while(!feof(fp)) {
        
        fgets(line, 256, fp); // read line

        if (0 == strncmp(line, "v ", 2)) {
            vec3f v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            darray_push(vertex_coord, v);
        } else if (0 == strncmp(line, "vt", 2)) {
            vec3f vt;
            sscanf(line, "vt %f %f %f", &vt.x, &vt.y, &vt.z);
            darray_push(texture_coord, vt);
        } else if (0 == strncmp(line, "vn", 2)) {
            vec3f vn;
            sscanf(line, "vn %f %f %f", &vn.x, &vn.y, &vn.z);
            darray_push(normal, vn);
        } else if (0 == strncmp(line, "f ", 2)) {
            vec3i v_indices, vt_indices, vn_indices;
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &v_indices.x, &vt_indices.x, &vn_indices.x,
                   &v_indices.y, &vt_indices.y, &vn_indices.y,
                   &v_indices.z, &vt_indices.z, &vn_indices.z);
            darray_push(vertex_indices, v_indices);
            darray_push(texture_indices, vt_indices);
            darray_push(normal_indices, vn_indices);
        }
    };

    mesh_t* mesh = mesh_new(vertex_coord, texture_coord, normal, 
                            vertex_indices, texture_indices, normal_indices);

    fclose(fp);

    return mesh;
}

static mesh_t* mesh_new(vec3f* vertex_coord, vec3f* texture_coord, vec3f* normal, 
                        vec3i* vertex_indices, vec3i* texture_indices, vec3i* normal_indices) {
    
    int triangles_number = darray_size(vertex_indices);
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
    triangle_t* triangles = (triangle_t*)malloc(sizeof(triangle_t) * triangles_number);

    for (int i = 0; i < triangles_number; i++) {
        
        triangles[i].vertex[0] = vec3_2_vec4(vertex_coord[vertex_indices[i].x - 1], 1);
        triangles[i].vertex[1] = vec3_2_vec4(vertex_coord[vertex_indices[i].y - 1], 1);
        triangles[i].vertex[2] = vec3_2_vec4(vertex_coord[vertex_indices[i].z - 1], 1);

        // uv's z is always 0
        triangles[i].uv[0] = texture_coord[texture_indices[i].x - 1];
        triangles[i].uv[1] = texture_coord[texture_indices[i].y - 1];
        triangles[i].uv[2] = texture_coord[texture_indices[i].z - 1];

        triangles[i].normal[0] = normal[normal_indices[i].x - 1];
        triangles[i].normal[1] = normal[normal_indices[i].y - 1];
        triangles[i].normal[2] = normal[normal_indices[i].z - 1];
        
    }

    mesh->triangles_Number = triangles_number;
    mesh->triangles = triangles;

    return mesh;
}

void mesh_free(mesh_t* mesh) {

    free(mesh->triangles);
    free(mesh);
}
