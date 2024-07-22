#include "image.h"

static void tga_save(FILE *file, image_t *image);
static int get_image_pixels(image_t *image);
static void load_rle_image(FILE *file, image_t *image);
static image_t *tga_load(FILE *file);
static void read_tga_header(FILE *file, int *width, int *height, int *channels,
                            bool *isRLE);

/**
 * @brief creat a image object
 *
 * @param width width's of the image.
 * @param height height's of the image.
 * @param channels channels of the image, it can 3 or 4 channels.
 * @return IMAGE
 * @file Image.c
 * @version 0.2
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-03
 * @copyright Copyright (c) 2024
 */
image_t *image_new(int width, int height, int channels) {
  image_t *image = (image_t *)malloc(sizeof(image_t));

  image->channels = channels;
  image->width = width;
  image->height = height;
  image->data = NULL;

  // Init image data
  int totalPixels = sizeof(unsigned char) * width * height * channels;
  image->data = (unsigned char *)malloc(totalPixels);
  memset(image->data, 0, totalPixels);

  // Init image color buffer(z-buffer)
  int depthBufferSize = sizeof(float) * width * height;
  image->depthBuffer = (float *)malloc(depthBufferSize);
  // memset(image->depthBuffer, Negative_Infinity, depthBufferSize);
  for (int i = 0; i < (width * height); i++) {
    image->depthBuffer[i] = Negative_Infinity;
  }

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
int get_image_position(image_t *image, int x, int y) {

    // position = (y * width * channels) + (x * channels)
    return (y * image->width * image->channels) + (x * image->channels);
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
void image_save(char const *filename, image_t *image) {
    
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
static void tga_save(FILE* file, image_t *image) {

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
    fwrite(image->data, 1, get_image_pixels(image), file);
   
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
void image_free(image_t *image) {
    free(image->data);
    free(image->depthBuffer);
    free(image);
}

/**
 * @brief load image file and read picture data.
 * 
 * @param filename 
 * @return IMAGE* 
 * @file image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-11
 * @copyright Copyright (c) 2024
 */
image_t *image_load(char const *filename) {
  
  FILE *file = fopen(filename, "rb");
  image_t *image = tga_load(file);
  fclose(file);

  return image;
}

/**
 * @brief
 *
 * @param file
 * @return image_t*
 * @file image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-11
 * @copyright Copyright (c) 2024
 */
static image_t *tga_load(FILE *file) {

    // read tga file header
    int width, height, channels;
    bool isRLE;
    read_tga_header(file, &width, &height, &channels, &isRLE);

    image_t *image = image_new(width, height, channels);

    if (isRLE) {
       load_rle_image(file, image);
    } else {
       fread(image->data, 1, get_image_pixels(image), file);
    }

    return image;
}

static void read_tga_header(FILE *file, int* width, int* height, int* channels, bool* isRLE) {
    
    unsigned char tgaHeader[18];
    fread(tgaHeader, 1, 18, file);
    
    int idLenth = tgaHeader[0];       // not use
    int colorMapType = tgaHeader[1];  // not use
    int imageType = tgaHeader[2];     // image type is 1, 2, 10, 11
    int firstEntryIndex = ((int)tgaHeader[4] << 8) + (int)tgaHeader[3]; // not use
    int colorMapLength = ((int)tgaHeader[6] << 8) + (int)tgaHeader[5];  // not use
    int colorMapEntrySize = tgaHeader[7];                               // not use
    int xOrigin = ((int)tgaHeader[9] << 8) + (int)tgaHeader[8];         // not use
    int yOrigin = ((int)tgaHeader[11] << 8) + (int)tgaHeader[10];       // not use
    int imageWidth = ((int)tgaHeader[13] << 8) + (int)tgaHeader[12];  // image width
    int imageHeight = ((int)tgaHeader[15] << 8) + (int)tgaHeader[14];  // image height
    int pixelDepth = tgaHeader[16];       // support 8, 16, 24, 32 bits
    int imageDescriptor = tgaHeader[17];  // not use

    *width = imageWidth; *height = imageHeight;
    *channels = pixelDepth / 8;
    *isRLE = ((10 == imageType) || (11 == imageType));
}

static int get_image_pixels(image_t *image) {
  return image->width * image->height * image->channels;
}

/**
 * @brief implement run-length-encoded algorithm
 * @ref RUN-LENGTH ENCODING OF IMAGES chapter of
 *      http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
 * @param file
 * @param image
 * @file image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-12
 * @copyright Copyright (c) 2024
 */
static void load_rle_image(FILE* file, image_t* image) {
    int totalPixels = get_image_pixels(image);
    int currentPixels = 0;

    while (currentPixels < totalPixels) {
        unsigned char firstFiled = fgetc(file);
        bool rlePacket = firstFiled & 0x80;
        int pixelCount = (firstFiled & 0x7F) + 1;

        if (rlePacket) { /* Run-Length Packet */
            unsigned char pixels[4];
            fread(pixels, 1, image->channels, file);
            for (int i = 0; i < pixelCount; i++) {
                for (int j = 0; j < image->channels; j++) {
                    image->data[currentPixels++] = pixels[j];
                }
            }
        } else { /* Raw Packet */
          for (int i = 0; i < pixelCount; i++) {
            for (int j = 0; j < image->channels; j++) {
              image->data[currentPixels++] = fgetc(file); 
            }
          }
        }
    }
}

/**
 * @brief convert obj vertex coordinates to image coordinates.
 *        obj vertex interval range is [-1, 1], image interval range is [1, x],
 *        but actually store image interval range is [0, x-1].
 *        `x` is the maximum value between image width and height.
 * @param point
 * @param image
 * @return vec2i
 * @file utils.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-13
 * @copyright Copyright (c) 2024
 */
vec2i convert_vertex_point(vec3f point, image_t *image) {
  return vec2_new(mapping_interval(-1, 1, 1, image->width, point.x) - 1,
                   mapping_interval(-1, 1, 1, image->width, point.y) - 1);
}