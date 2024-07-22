#include "../core/common.h"
#include "../core/api.h"
#include "../core/macro.h"

int main() {
    
    int width = 800, height = 800;
    windows_t* window = windows_create("Renderer", width, height);

    image_t* image = image_new(width, height, 3);
    // Run the message loop.
    while (!windows_is_closed(window)) {

        RGB red = {255, 0, 0};
        mesh_t* mesh = mesh_load("../assets/african_head/african_head.obj");

        for (int i = 0; i < mesh->v_indices_size; i++) {
            
            int v_indices[3] = {mesh->v_indices[i].x - 1, 
                                mesh->v_indices[i].y - 1, 
                                mesh->v_indices[i].z - 1};
            for (int j = 0; j < 3; j++) {
                vec3f v0 = mesh->v[v_indices[j]];
                vec3f v1 = mesh->v[v_indices[(j + 1) % 3]];
                vec2i point0 = vec2_new(mapping_interval(-1, 1, 1, width, v0.x),
                                            mapping_interval(-1, 1, 1, width, v0.y));
                vec2i point1 = vec2_new(mapping_interval(-1, 1, 1, width, v1.x),
                                            mapping_interval(-1, 1, 1, width, v1.y));
                draw2d_line(point0, point1, red, image);
            } 
        }
        
        // image_save("output.tga", image);

        windows_draw_buffer(window, image);

        windows_message_loop();
    }

    destory_windows(window);
    return 0;
}