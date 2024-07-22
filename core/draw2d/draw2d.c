#include "draw2d.h"

static void swap_point(vec2i *p0, vec2i *p1);
static void draw2d_point_utils(int x, int y, unsigned char *color, image_t *image);
static void draw2d_line_utils(vec2i p0, vec2i p1, unsigned char *color, image_t *image);
static void draw2d_triangle_raster_utils(vec2i p0, vec2i p1, vec2i p2,
                                         vec3f vt0, vec3f vt1, vec3f vt2,
                                         texture_t *texture, image_t *image,
                                         vec3f depthBuffer, unsigned char *color, float intensity);
typedef struct {vec2i min, max;} bbox;
static bbox find_bounding_box(vec2i p0, vec2i p1, vec2i p2);
static int get_depthBuffer_postion(vec2i p, image_t *image);
static float get_zbuffer_value(vec3f depthBuffer, vec3f barycoord);

/**
 * @brief write a pixel(3 or 4 channels) to image(matrix).
 *
 * @param p0  point coordinate(0< x < width-1, 0< y < height-1).
 * @param color RGB or RGBA
 * @param image
 * @file Image.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-03
 * @copyright Copyright (c) 2024
 */
void draw2d_point(vec2i p0, unsigned char *color, image_t *image) {
  // convert point 
  int x = p0.x - 1;
  int y = p0.y - 1;

  // draw point
  draw2d_point_utils(x, y, color, image);
}

static void draw2d_point_utils(int x, int y, unsigned char *color, image_t *image) {

  // (y * height * channels) + (x * channels);
  int position = get_image_position(image, x, y);
  int totalPixels = image->width * image->height * image->channels;
  
  if (position <= (totalPixels - image->channels) && position > 0) {
    for (int i = 0; i < image->channels; i++) {
      image->data[position + i] = color[i];
    }

    // rgb to bgr
    if (image->channels >= 3) {
      swap_char((image->data + position), (image->data + position + 2));
    }
  }
}

/**
 * @brief draw line segment to image
 * @ref  see Bresenham's line algorithm from
 *       https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf
 *
 * @param p0 start coordinate of line segment
 * @param p1 end coordinate of line segment
 * @param color
 * @param image
 * @file draw2d.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-01-06
 * @copyright Copyright (c) 2024
 */
void draw2d_line(vec2i p0, vec2i p1, unsigned char *color, image_t *image) {

  // convert point
  vec2i _p0 = vec2_new(p0.x - 1, p0.y - 1);
  vec2i _p1 = vec2_new(p1.x - 1, p1.y - 1);

  // draw line
  draw2d_line_utils(_p0, _p1, color, image);
}

static void draw2d_line_utils(vec2i p0, vec2i p1, unsigned char *color, image_t *image) {

  int x, y;

  /* vertical line */
  if (0 == (p1.x - p0.x)) {
    for (x = p0.x, y = p0.y; y < p1.y; y++) {
      draw2d_point_utils(x, y, color, image);
    }
  /* horizontal line */
  } else if (0 == (p1.y - p0.y)) {
    for (x = p0.x, y = p0.y; x < p1.x; x++) {
      draw2d_point_utils(x, y, color, image);
    }
  } else {
    /* x0>x1 */
    if (p0.x > p1.x) {
      swap_point(&p0, &p1);
    }
    /* k<0 */
    bool mirror = false;
    if ((float)(p1.y - p0.y) / (float)(p1.x - p0.x) < 0) {
      p0.y = -p0.y;
      p1.y = -p1.y;
      mirror = true;
    }

    /* k>1 */
    bool steep = false;
    if ((float)(p1.y - p0.y) / (float)(p1.x - p0.x) > 1) {
      swap_int(&p0.x, &p0.y);
      swap_int(&p1.x, &p1.y);
      steep = true;
    }

    /* x0<x1 0<k<1 */
    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);
    int d = 2 * dy - dx;

    for (x = p0.x, y = p0.y; x < p1.x; x++) {
      if ((true == steep) && (false == mirror)) {
        draw2d_point_utils(y, x, color, image);
      } else if ((false == steep) && (false == mirror)) {
        draw2d_point_utils(x, y, color, image);
      } else if ((false == steep) && (true == mirror)) {
        draw2d_point_utils(x, -y, color, image);
      } else if ((true == steep) && (true == mirror)) {
        draw2d_point_utils(y, -x, color, image);
      }
      if (d < 0) {
        d += incrE;
      } else {
        y++;
        d += incrNE;
      }
    }
  }

}

static void swap_point(vec2i *p0, vec2i *p1) {
  swap_int(&p0->x, &p1->x);
  swap_int(&p0->y, &p1->y);
}

/**
 * @brief convert point coordinates and draw raster triangles to image.
 *
 * @param p0 `p0`, `p1` and `p2` are the vertices coordinates.
 * @param p1
 * @param p2
 * @param vt0 `vt0`, `vt1` and `vt2` are the texture coordinates.
 * @param vt1
 * @param vt2
 * @param texture
 * @param image
 * @param color RGB、RGBA、black-white、black-white-alpha.
 * @param intensity light intensity.
 * @file draw2d.c
 * @version 0.3
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-13
 * @copyright Copyright (c) 2024
 */
void draw2d_triangle_raster(vec3f p0, vec3f p1, vec3f p2,
                            vec3f vt0, vec3f vt1, vec3f vt2,
                            texture_t* texture, image_t *image,
                            unsigned char *color, float intensity) {
  // convert point
  vec2i _p0 = convert_vertex_point(p0, image);
  vec2i _p1 = convert_vertex_point(p1, image);
  vec2i _p2 = convert_vertex_point(p2, image);

  vec3f _vt0 = convert_texture_point(vt0, texture);
  vec3f _vt1 = convert_texture_point(vt1, texture);
  vec3f _vt2 = convert_texture_point(vt2, texture);

  vec3f depthBuffer = vec3_new(p0.z, p1.z, p2.z);

  // draw raster triangles
  draw2d_triangle_raster_utils(_p0, _p1, _p2, _vt0, _vt1, _vt2,
                               texture, image, 
                               depthBuffer, color, intensity);
}


/**
 * @brief draw raster triangles to image.
 * @ref   https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
 *        https://zhuanlan.zhihu.com/p/523501661
 * @param p0 `p0`, `p1` and `p2` are the vertices coordinates.
 * @param p1
 * @param p2
 * @param vt0 `vt0`, `vt1` and `vt2` are the texture coordinates.
 * @param vt1
 * @param vt2
 * @param texture
 * @param image
 * @param depthBuffer z-buffer value from three vertex coordinates.
 * @param color RGB、RGBA、black-white、black-white-alpha.
 * @param intensity light intensity
 * @file draw2d.c
 * @version 0.5
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-13
 * @copyright Copyright (c) 2024
 */
static void draw2d_triangle_raster_utils(vec2i p0, vec2i p1, vec2i p2,
                                         vec3f vt0, vec3f vt1, vec3f vt2,
                                         texture_t *texture, image_t *image,
                                         vec3f depthBuffer, unsigned char *color, float intensity) {
  // find bounding box AABB（Axis-Aligned Bounding Boxes)
  bbox AABB_box = find_bounding_box(p0, p1, p2);

  // raster triangles
  for (int x = AABB_box.min.x; x < AABB_box.max.x; x++) {
    for (int y = AABB_box.min.y; y < AABB_box.max.y; y++) {
      vec2i p = {x, y};
      vec3f barycoord = barycentric(p, p0, p1, p2);
      // handling accuracy issues. 
      // if `-0.01` is `0`, maybe some point from model will discard in rendering.
      if (barycoord.x < -0.01 || barycoord.y < -0.01 || barycoord.z < -0.01) {
        continue; 
      }
      // calculate z-buffer values
      float perPixelDepth = get_zbuffer_value(depthBuffer, barycoord);
      int colorPos = get_depthBuffer_postion(p, image);
      if (image->depthBuffer[colorPos] < perPixelDepth) {
        
        // interpolate color values from texture coordinates
        vec3f _texCoord = vec3_add3(vec3_mult(vt0, barycoord.x),
                                     vec3_mult(vt1, barycoord.y),
                                     vec3_mult(vt2, barycoord.z));
        vec2i texCoord = vec2_new((int)(_texCoord.x), (int)(_texCoord.y));
        get_texture_pixel(texture, color, texCoord);
        calculate_lighting_color(texture, color, intensity);
        image->depthBuffer[colorPos] = perPixelDepth;
        draw2d_point_utils(p.x, p.y, color, image);
      }
    }
  }
}

/**
 * @brief implement a AABB（Axis-Aligned Bounding Boxes) box
 *
 * @param p0
 * @param p1
 * @param p2
 * @return bbox
 * @file draw2d.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-10
 * @copyright Copyright (c) 2024
 */
static bbox find_bounding_box(vec2i p0, vec2i p1, vec2i p2) {
  int x[3] = {p0.x, p1.x, p2.x};
  int y[3] = {p0.y, p1.y, p2.y};

  bubble_sort(x, 3);
  bubble_sort(y, 3);

  bbox box = {vec2_new(x[0], y[0]), vec2_new(x[2], y[2])};

  return box;
}

static int get_depthBuffer_postion(vec2i p, image_t *image) {
  return p.x + p.y * image->width;
}

/**
 * @brief Get the zbuffer value object
 *
 * @param depthBuffer it is always three from three vertex coordinates'z values.
 * @param barycoord
 * @return float
 * @file draw2d.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-10
 * @copyright Copyright (c) 2024
 */
static float get_zbuffer_value(vec3f depthBuffer, vec3f barycoord) {
  return (depthBuffer.x * barycoord.x + depthBuffer.y * barycoord.y +
          depthBuffer.z * barycoord.z);
}