#ifndef WIN32_H
#define WIN32_H

#include <windows.h>
#include "../common.h"
#include "../api.h"

typedef struct {
    HWND hwnd;
    HDC  memory_dc;
    image_t* image;

    boolean is_close;
}windows_t;

windows_t* windows_create(const char* title, int width, int height);
boolean windows_is_closed(windows_t* window);
void windows_draw_buffer(windows_t* window, image_t* image);
void windows_message_loop();
void destory_windows(windows_t* window);

#endif