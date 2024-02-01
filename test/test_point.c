#include "../core/api.h"
#include <stdlib.h>


int main(int argc, char **argv) {

    RGBA red = {255, 0, 0, 255}; // actuall bgr
    IMAGE* image = image_init(100, 100, 4);
    vec2_i p = {1, 3};
    draw2d_point(p, red, image);
    // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image_save("output.tga", image);
    image_free(image);
    return 0;
}
