#include "graphics.h"

static void graphics_triangle_raster_utils(program_t* program, texture_t* texture, image_t* image);
static void graphics_point_utils(int x, int y, vec4f _color, image_t *image);

typedef struct {vec2i min, max;} bbox_t;
static bbox_t find_bounding_box(vec2i p0, vec2i p1, vec2i p2);
static int get_depthBuffer_postion(vec2i p, image_t *image);
static float zbuffer_interp(vec3f depthBuffer, vec3f barycoord);
static bool is_back_face(vec3f v[3]);

program_t* program_new(vertex_shader_t* vertex_shader, fragment_shader_t* fragment_shader,
                       int attributeSize, int varyingSize, int uniformSize) {
    
    program_t* program = (program_t*)malloc(sizeof(program_t));
    
    program->vertex_shader = vertex_shader;
    program->fragment_shader = fragment_shader;
    program->varingSize = varyingSize;
    
    for (int i = 0; i < 3; i++) {
      program->attribute[i] = malloc(attributeSize);
      // memset(program->attribute[i], 0, attributeSize);
    }
    for (int i = 0; i < 3; i++) {
      program->varying_buffer[i] = malloc(varyingSize);
      // memset(program->varying_buffer[i], 0, varyingSize);
    }
    program->varying = malloc(varyingSize);
    // memset(program->varying, 0, varyingSize);
    program->uniform = malloc(uniformSize);
    // memset(program->uniform, 0, uniformSize);

    return program;
}

void program_free(program_t* program) {
    free(program->attribute);
    free(program->varying);
    free(program->uniform);
    free(program);
}

void graphics_triangle_raster(program_t* program, texture_t* texture, image_t *image) {
  
  // convert ndc point
  vec4f clip_coord[3];
  vec3f ndc_v[3];
  for (int i = 0; i < 3; i++) {
    clip_coord[i] = program->vertex_shader(program->attribute[i], program->varying_buffer[i], program->uniform);
    ndc_v[i] = vec4_2_vec3(clip_coord[i], clip_coord[i].w);
  }
  
  // w value in homogeneous coordinates
  program->zValue = vec3_new(clip_coord[0].w, clip_coord[1].w, clip_coord[2].w);

  // convert screen point and store z-buffer value
  vec3f screen_v[3];
  mat4f viewport = mat4_viewport(image->width, image->height);
  for (int i = 0; i < 3; i++) {
    vec4f tmp= mat4_mult_vec4(viewport, vec3_2_vec4(ndc_v[i], 1));
    program->screen_coord[i] = vec3_div(vec4_2_vec3(tmp, 1), 1);
  }

  if (!is_back_face(ndc_v)) {
    // draw raster triangles
    graphics_triangle_raster_utils(program, texture, image);
  }
}

static void graphics_triangle_raster_utils(program_t* program, texture_t* texture, image_t* image) {
  
  vec2i v[3];
  for (int i = 0; i < 3; i++) {
    v[i] = vec2_new(program->screen_coord[i].x, program->screen_coord[i].y);
  }

  // find bounding box AABB（Axis-Aligned Bounding Boxes)
  bbox_t AABB_box = find_bounding_box(v[0], v[1], v[2]);

  // raster triangles
  for (int x = AABB_box.min.x; x < AABB_box.max.x; x++) {
    for (int y = AABB_box.min.y; y < AABB_box.max.y; y++) {
      vec2i p = {x, y};

      vec3f barycoord = barycentric(p, v[0], v[1], v[2]);
      // handling accuracy issues. 
      // if `-0.01` is `0`, maybe some point from model will discard in rendering.
      if (barycoord.x < -0.01 || barycoord.y < -0.01 || barycoord.z < -0.01) {
        continue; 
      }
      
      // calculate z-buffer values
      float perPixelDepth = zbuffer_interp(program->zValue, barycoord);
      int colorPos = get_depthBuffer_postion(p, image);
      if (image->depthBuffer[colorPos] < perPixelDepth) {
        
        image->depthBuffer[colorPos] = perPixelDepth;

        // interpolate color values from texture coordinates
        perspective_correct_interp2(program->varying_buffer, program->varying, program->varingSize, barycoord, program->zValue);
        vec4f color = program->fragment_shader(program->varying, program->uniform, texture);

        graphics_point_utils(p.x, p.y, color, image);
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
static bbox_t find_bounding_box(vec2i p0, vec2i p1, vec2i p2) {
  int x[3] = {p0.x, p1.x, p2.x};
  int y[3] = {p0.y, p1.y, p2.y};

  bubble_sort(x, 3);
  bubble_sort(y, 3);

  bbox_t box = {vec2_new(x[0], y[0]), vec2_new(x[2], y[2])};

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
static float zbuffer_interp(vec3f depthBuffer, vec3f barycoord) {
  return (depthBuffer.x * barycoord.x + depthBuffer.y * barycoord.y +
          depthBuffer.z * barycoord.z);
}

static void graphics_point_utils(int x, int y, vec4f color, image_t *image) {
   
  // (y * height * channels) + (x * channels);
  int position = get_image_position(image, x, y);
  int totalPixels = image->width * image->height * image->channels;
  
  if (position <= (totalPixels - image->channels) && position > 0) {
    image->data[position + 0] = (unsigned char)color.x;
    image->data[position + 1] = (unsigned char)color.y;
    image->data[position + 2] = (unsigned char)color.z;

    // rgb to bgr
    if (image->channels >= 3) {
      swap_char((image->data + position), (image->data + position + 2));
    }
  }
}

/******************************************************************************
 * @brief assume look at negative z-axis
 * 
 * @param v 
 * @return true 
 * @return false 
 * @file graphics.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-08-05
 * @copyright Copyright (c) 2024
******************************************************************************/
static bool is_back_face(vec3f v[3]) {
    //        --->     --->
    // vector v0v1 and v0v2 can't switch positions
    vec3f normal = vec3_cross(vec3_sub(v[2], v[0]), vec3_sub(v[1], v[0]));
    vec3f z = vec3_new(0, 0, -1);
    return vec3_dot(normal, z) <= 0;
}