#include "../core/api.h"

int main(int argc, char **argv) {

    RGB red = {255, 0, 0};
    RGB white = {255, 255, 255};

    int width = 1000;
    int height = 1000;
    IMAGE* image = image_init(width, height, 3);

    /* 
    vec2_i point0 = point_create(13, 20);
    vec2_i point1 = point_create(80, 40);
    // line(13, 20, 80, 40, image, white);
    draw2d_line(point0, point1, white, image);

    // line(20, 13, 40, 80, image, red);
    point_set(&point0, 20, 13);
    point_set(&point1, 40, 80);
    draw2d_line(point0, point1, red, image);

    // line(80, 40, 13, 20, image, red);
    point_set(&point0, 80, 40);
    point_set(&point1, 13, 20);
    draw2d_line(point0, point1, red, image);
    */

    vec2_i point0 = point_create(((-0.9 + 1.0) * width / 2.0),
                                 ((0.9 + 1.0) * height / 2.0));
    vec2_i point1 = point_create(((0.5 + 1.0) * width / 2.0),
                                 ((-0.5 + 1.0) * height / 2.0));
    draw2d_line(point0, point1, red, image);
    // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image_save("output.tga", image);
    return 0;
}
