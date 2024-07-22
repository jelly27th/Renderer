#include "../core/api.h"
#include <stdlib.h>


int main(int argc, char **argv) {

    RGBA red = {255, 0, 0, 255}; // actuall bgr
    image_t* image = image_new(100, 100, 4);
    vec2i p = {1, 3};
    draw2d_point(p, red, image);
    image_save("output.tga", image);
    image_free(image);
    return 0;
}
