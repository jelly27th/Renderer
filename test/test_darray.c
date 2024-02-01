#include "../core/api.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    vec2_i* foo = NULL;
    
    vec2_i t1 = {1, 2};
    vec2_i t2 = {3, 4};
    vec2_i t3 = {5, 6};
    vec2_i t4 = {7, 8};
    vec2_i t5 = {9, 10};
    vec2_i t6 = {11, 12};
    vec2_i t7 = {13, 14};

    darray_push(foo, t1);
    printf("%d %d %d\n", foo[0].x, foo[0].y, darray_size(foo));
    darray_push(foo, t2);
    printf("%d %d %d\n", foo[1].x, foo[1].y, darray_size(foo));
    darray_push(foo, t3);
    printf("%d %d %d\n", foo[2].x, foo[2].y, darray_size(foo));
    darray_push(foo, t4);
    printf("%d %d %d\n", foo[3].x, foo[3].y, darray_size(foo));
    darray_push(foo, t5);
    printf("%d %d %d\n", foo[4].x, foo[4].y, darray_size(foo));
    darray_push(foo, t6);
    printf("%d %d %d\n", foo[5].x, foo[5].y, darray_size(foo));
    darray_push(foo, t7);
    printf("%d %d %d\n", foo[6].x, foo[6].y, darray_size(foo));

    for (int i = 0; i < 7; i++) {
        printf("%d %d\n", foo[i].x, foo[i].y);
    }

    darray_free(foo);

    return 0;
}