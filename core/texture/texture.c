#include "texture.h"

static void get_texture_pixel_utils(texture_t* texture, unsigned char* color, int x, int y);

texture_t* texture_new(int width, int height, int channels) {
  
    texture_t* texture = (texture_t*)malloc(sizeof(texture_t));

    texture->channels = channels;
    texture->width = width;
    texture->height = height;
    texture->pixels = NULL;

    // Init image data
    int totalPixels = sizeof(unsigned char) * width * height * channels;
    texture->pixels = (unsigned char*)malloc(totalPixels);
    // memset(texture->pixels, 0, totalPixels);
}

texture_t* image_to_texture(image_t* image) {

    texture_t* texture = texture_new(image->width, image->height, image->channels);
    // copy data from deep
    // strcpy(texture->pixels, image->data);
    int totalPixels = image->width * image->height * image->channels;
    for (int i = 0; i < totalPixels; i++) {
        texture->pixels[i] = image->data[i];
    }

    return texture;
}

void texture_free(texture_t* texture) {
    free(texture->pixels);
    free(texture);
}

int get_texture_position(texture_t* texture, int x, int y) {
  // position = (y * height * channels) + (x * channels)
  return (y * texture->height * texture->channels) + (x * texture->channels);
}

/**
 * @brief Get the texture pixel object
 * 
 * @param texture 
 * @param color 
 * @param x 
 * @param y 
 * @file texture.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-12
 * @copyright Copyright (c) 2024
 */
void get_texture_pixel(texture_t* texture, unsigned char* color, vec2i p) {

    // convert texture coordinates
    // int x = p.x - 1;
    // int y = p.y - 1;

    get_texture_pixel_utils(texture, color, p.x, p.y);
}

static void get_texture_pixel_utils(texture_t* texture, unsigned char* color, int x, int y) {

    int position = get_texture_position(texture, x, y);

    for (int i = 0; i < texture->channels; i++) {
        color[i] = texture->pixels[position++];
    }

    // bgr to rgb
    if (texture->channels >= 3) {
      swap_char(color, color+2);
    }
}

void calculate_lighting_color(texture_t* texture, unsigned char* color, float intensity) {
    for (int i = 0; i < texture->channels; i++) {
        color[i] = intensity * color[i];
    }
}

/**
 * @brief convert obj texture coordinates to image coordinates.
 *        obj texture interval range is [0, 1], image interval range is [1, x],
 *        but actually store image interval range is [0, x-1].
 *        `x` is the maximum value between image width and height.
 *
 * @param point
 * @param texture
 * @return vec3f
 * @file utils.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-13
 * @copyright Copyright (c) 2024
 */
vec3f convert_texture_point(vec3f point, texture_t* texture) {
  return vec3_new(mapping_interval(0, 1, 1, texture->width, point.x) - 1,
                   mapping_interval(0, 1, 1, texture->width, point.y) - 1, 0.0);
}