#include "draw2d.h"

static void swap_point(vec2i *p0, vec2i *p1);
static void draw2d_point_utils(int x, int y, unsigned char *color, IMAGE *image);
static void draw2d_line_utils(vec2i p0, vec2i p1, unsigned char *color, IMAGE *image);
static void draw2d_triangle_raster_utils(vec2i p0, vec2i p1, vec2i p2,
                                         unsigned char *color, IMAGE *image);

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
void draw2d_point(vec2i p0, unsigned char *color, IMAGE *image) {
  // convert point 
  int x = p0.x - 1;
  int y = p0.y - 1;

  // draw point
  draw2d_point_utils(x, y, color, image);
}

static void draw2d_point_utils(int x, int y, unsigned char *color, IMAGE *image) {

  // (y * height * channels) + (x * channels);
  int position = get_image_position(image, x, y);

  for (int i = 0; i < image->channels; i++) {
      image->data[position + i] = color[i];
  }

  // rgb to bgr
  if (image->channels >= 3) {
      swap_char((image->data + position), (image->data + position + 2));
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
void draw2d_line(vec2i p0, vec2i p1, unsigned char *color, IMAGE *image) {

  // convert point
  vec2i _p0 = vec2i_new(p0.x - 1, p0.y - 1);
  vec2i _p1 = vec2i_new(p1.x - 1, p1.y - 1);

  // draw line
  draw2d_line_utils(_p0, _p1, color, image);
}

static void draw2d_line_utils(vec2i p0, vec2i p1, unsigned char *color, IMAGE *image) {

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
 * @brief draw raster triangles to image.
 *        for more information see
 *        https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
 *        https://zhuanlan.zhihu.com/p/523501661
 *
 * @param p0 one of the triangles coordinates
 * @param p1 one of the triangles coordinates
 * @param p2 one of the triangles coordinates
 * @param color
 * @param image
 * @file draw2d.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-07
 * @copyright Copyright (c) 2024
 */
void draw2d_triangle_raster(vec2i p0, vec2i p1, vec2i p2,
                            unsigned char *color, IMAGE *image) {
  // convert point
  vec2i _p0 = vec2i_new(p0.x - 1, p0.y - 1);
  vec2i _p1 = vec2i_new(p1.x - 1, p1.y - 1);
  vec2i _p2 = vec2i_new(p2.x - 1, p2.y - 1);
  // draw raster triangles
  draw2d_triangle_raster_utils(_p0, _p1, _p2, color, image);
}

static void draw2d_triangle_raster_utils(vec2i p0, vec2i p1, vec2i p2,
                                         unsigned char *color, IMAGE *image) {
  // find bounding box AABB（Axis-Aligned Bounding Boxes)
  int x[3] = {p0.x, p1.x, p2.x};
  int y[3] = {p0.y, p1.y, p2.y};
  vec2i AABB_box_min, AABB_box_max;

  bubble_sort(x, 3);
  bubble_sort(y, 3);
  AABB_box_min.x = x[0]; AABB_box_max.x = x[2];
  AABB_box_min.y = y[0]; AABB_box_max.y = y[2];

  // raster triangles
  for (int _x = AABB_box_min.x; _x < AABB_box_max.x; _x++) {
    for (int _y = AABB_box_min.y; _y < AABB_box_max.y; _y++) {
      vec2i p = {_x, _y};
      vec3f barycoord = barycentric(p, p0, p1, p2);
      // handling accuracy issues. 
      // if `-0.1` is `0`, maybe some point from model will discard in rendering.
      if (barycoord.x < -0.1 || barycoord.y < -0.1 || barycoord.z < -0.1) {
        continue; 
      }
      draw2d_point_utils(p.x, p.y, color, image);
    }
  }
}