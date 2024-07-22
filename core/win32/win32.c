#include "win32.h"

static const char *const WINDOW_CLASS_NAME = "Class";
static const char *const WINDOW_ENTRY_NAME = "Entry";

static void register_windows_class(void);
static LRESULT CALLBACK process_messgae(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HWND create_windows(const char* title, int width, int height);
static void create_bitmap(HWND hwnd, int width, int height, image_t** out_image, HDC* out_memory_dc);
static void update_bitmap(windows_t* window);
static void windows_copy_buffer(image_t* dstbuffer, image_t* srcbuffer);

/******************************************************************************
 * @brief 
 * 
 * @param title 
 * @param width 
 * @param height 
 * @return windows_t* 
 * @file win32.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-07-01
 * @copyright Copyright (c) 2024
******************************************************************************/
windows_t* windows_create(const char* title, int width, int height) {
    
    // Register the window class.
    register_windows_class();

    // Create the window.
    HWND hwnd = create_windows(title, width, height);

    // Initalize bitmap
    image_t* image;
    HDC memory_dc;
    create_bitmap(hwnd, width, height, &image, &memory_dc);
 
    windows_t* window = (windows_t*)malloc(sizeof(windows_t));
    window->hwnd = hwnd;
    window->memory_dc = memory_dc;
    window->image = image;
    window->is_close = false;

    SetProp(hwnd, WINDOW_ENTRY_NAME, window);
    // Show the window
    ShowWindow(hwnd, SW_SHOW);

    return window;
}

static void register_windows_class(void) {
    WNDCLASS wc = {0};

    wc.lpfnWndProc = process_messgae;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&wc);
}

static LRESULT CALLBACK process_messgae(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    windows_t *window = (windows_t*)GetProp(hwnd, WINDOW_ENTRY_NAME);
    switch (uMsg) {
        case WM_CLOSE:
            window->is_close = true;
            return 0;
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static HWND create_windows(const char* title, int width, int height) {

    HWND hwnd = CreateWindowEx( 0,                                       // Optional window styles.
                            WINDOW_CLASS_NAME,                           // Window class
                            title,                                       // Window text
                            WS_OVERLAPPEDWINDOW,                         // Window style
                            CW_USEDEFAULT, CW_USEDEFAULT, width, height, // Size and position
                            NULL,                                        // Parent window    
                            NULL,                                        // Menu
                            GetModuleHandle(NULL),                       // Instance handle
                            NULL                                         // Additional application data
    );

    return hwnd;
}

static void create_bitmap(HWND hwnd, int width, int height, image_t** out_image, HDC* out_memory_dc) {

    HDC window_dc = GetDC(hwnd);
    HDC memory_dc = CreateCompatibleDC(window_dc);

    BITMAPINFO bitmap_info = {0};
    bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biWidth = width;
    bitmap_info.bmiHeader.biHeight = height;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
 
    image_t* image = image_new(width, height, 4);
    HBITMAP DIB_bitmap = CreateDIBSection(memory_dc, &bitmap_info, DIB_RGB_COLORS,
                                            (void**)&image->data, NULL, 0);
    HBITMAP old_bitmap = (HBITMAP)SelectObject(memory_dc, DIB_bitmap);

    ReleaseDC(hwnd, window_dc);
    DeleteObject(old_bitmap);

    *out_image = image;
    *out_memory_dc = memory_dc;
}

boolean windows_is_closed(windows_t* window) {
    return window->is_close;
}

void windows_draw_buffer(windows_t* window, image_t* image) {
    windows_copy_buffer(window->image, image); 
    update_bitmap(window);
}

static void windows_copy_buffer(image_t* dstbuffer, image_t* srcbuffer) {
    int width = srcbuffer->width;
    int height = srcbuffer->height;
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int src_index = get_image_position(srcbuffer, x, y);
            int dst_index = get_image_position(dstbuffer, x, y);
            dstbuffer->data[dst_index + 0] = srcbuffer->data[src_index + 0];
            dstbuffer->data[dst_index + 1] = srcbuffer->data[src_index + 1];
            dstbuffer->data[dst_index + 2] = srcbuffer->data[src_index + 2];
        }
    }
}

static void update_bitmap(windows_t* window) {
    HWND hwnd = window->hwnd;
    int width = window->image->width;
    int height = window->image->height;
    HDC memory_dc = window->memory_dc;

    HDC window_dc = GetDC(window->hwnd);
    BitBlt(window_dc, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, window_dc);
}

void windows_message_loop(void) {
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void destory_windows(windows_t* window) {
    
    ShowWindow(window->hwnd, SW_HIDE);
    RemoveProp(window->hwnd, WINDOW_ENTRY_NAME);
    DeleteDC(window->memory_dc);
    
    window->image->data = NULL;
    image_free(window->image);
    free(window);
    DestroyWindow(window->hwnd);
}

