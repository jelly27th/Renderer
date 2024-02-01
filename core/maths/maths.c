#include "maths.h"

vec2_i point_create(int x, int y) {
    vec2_i point;

    point.x = x; 
    point.y = y;
    
    return point;
}

void point_set(vec2_i *point, int x, int y) {
    point->x = x;
    point->y = y;
}