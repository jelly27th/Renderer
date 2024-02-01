#include "image.h"

static void tga_save(FILE* file, IMAGE *image);

/**
 * @brief creat a image object
 * 
 * @param width width's of the image.
 * @param height height's of the image.
 * @param channels channels of the image, it can 3 or 4 channels.
 * @return IMAGE 
 * @file Image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-03
 * @copyright Copyright (c) 2024
 */
IMAGE* image_init(int width, int height, int channels) {
    
    IMAGE *image = (IMAGE*)malloc(sizeof(IMAGE));

    image->channels = channels;
    image->width = width;
    image->height = height;
    image->data = NULL;

    // Init image data
    int totalPixels = sizeof(unsigned char) * width * height * channels;
    image->data = (unsigned char*)malloc(totalPixels);
    memset(image->data, 0, totalPixels);

    return image;

}

/**
 * @brief Get the image position object
 *
 * @param image
 * @param x Write pixels into the x-coordinate of the image(0< x < width-1).
 * @param y Write pixels into the y-coordinate of the image(0< y < height-1).
 * @return int
 * @file Image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-06
 * @copyright Copyright (c) 2024
 */
int get_image_position(IMAGE *image, int x, int y) {

    // position = (y * height * channels) + (x * channels)
    return (y * image->height * image->channels) + (x * image->channels);
}

/**
 * @brief save image data as a tga file.
 *        it follows that (1,1) is the bottom left corner.
 *
 * @param filename
 * @param image
 * @file Image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-03
 * @copyright Copyright (c) 2024
 */
void image_save(char const *filename, IMAGE *image) {
    
    FILE* file = fopen(filename, "wb");
    tga_save(file, image);
    fclose(file);
}

/**
 * @brief it can save a tga images, image attributes as follows:
 *        image type: support uncompressed true-color image and uncompressed black-and-white image
 *        pixel depth: support for 8、16、24、32 bit.
 *
 * @ref http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
 * @param filename
 * @param image
 * @file Image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-04
 * @copyright Copyright (c) 2024
 */
static void tga_save(FILE* file, IMAGE *image) {

    // write tga header to file
    unsigned char tgaHeader[18];
    memset(tgaHeader, 0, 18);
    tgaHeader[2] = image->channels != 1 ? 2 : 3;                        // image type 
    tgaHeader[12] = (unsigned char)(image->width & 0xFF);               // image width lsb
    tgaHeader[13] = (unsigned char)((image->width >> 8) & 0xFF);        // image width msb
    tgaHeader[14] = (unsigned char)(image->height & 0xFF);              // image height lsb
    tgaHeader[15] = (unsigned char)((image->height >> 8) & 0xFF);       // image height msb
    tgaHeader[16] = (unsigned char)((image->channels * 8) & 0xFF);      // pixel depth
    tgaHeader[17] = (image->channels == 2 || image->channels == 4) * 8; // image descriptor
    fwrite(tgaHeader, 1, 18, file);

    // write image data to file
    int totalPixels = image->width * image->height * image->channels;
    fwrite(image->data, 1, totalPixels, file);
   
}

/**
 * @brief destory image object
 * 
 * @param image 
 * @file Image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-05
 * @copyright Copyright (c) 2024
 */
void image_free(IMAGE *image) {
    free(image->data);
    free(image);
}