#ifndef _DARRAY_H
#define _DARRAY_H

#include "../common.h"

/**
 * @brief
 *  dynamic array's data structure as follows:
 *        0         1       index....
 *   -----------------------------------------------------------
 *  |  capacity | length |  data0 | data1 | data2 | data3.....
 *   -----------------------------------------------------------
 * for more information to https://blog.csdn.net/ALonWol/article/details/80564273
 *
 * @file darray.h
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-02
 * @copyright Copyright (c) 2024
 *
 */
#define darray_raw_address(darray) ((int *)(darray)-2)
#define darray_size(darray) (darray_raw_address(darray)[1])
#define darray_capacity(darray) (darray_raw_address(darray)[0])

static void *darray_resize(void *darray, int increment, int size);
#define darray_free(darray) (free(darray_raw_address(darray)))

#define darray_push(darray, value)                                          \
    do {                                                                    \
        (darray) = darray_resize(darray, 1, sizeof(*(darray)));             \
        (darray)[darray_size(darray) - 1] = (value);                        \
    } while (0)                                                             

static void *darray_resize(void *darray, int increment, int itemsize) {

    int* ptr = NULL;
    if (NULL == darray) {
        int size = itemsize * increment + sizeof(int) * 2;
        ptr = (int *)malloc(size);
        ptr[0] = increment; // capacity
        ptr[1] = increment; // length
    } else if (darray_size(darray) + increment <= darray_capacity(darray)) {
        darray_size(darray) += increment; // length
        ptr = darray_raw_address(darray);
    } else {
        int resize = darray_size(darray) + increment;
        int doubleSize = darray_capacity(darray) * 2;
        int capacity = resize > doubleSize ? resize : doubleSize;
        int size = itemsize * capacity + sizeof(int) * 2;
        ptr = (int *)realloc(darray_raw_address(darray), size);
        ptr[0] = capacity;   // capacity
        ptr[1] = resize;     // length
    }

    return ptr + 2;
}

#endif