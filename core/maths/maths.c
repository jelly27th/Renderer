#include "maths.h"

vec2i vec2_new(int x, int y) {
  vec2i point = {x, y};
  return point;
}

void vec2_reset(vec2i *point, int x, int y) {
  point->x = x;
  point->y = y;
}

/**                         --->    --->
 * @brief Build two vectors v0p and v0v1 and
 *        Check if the point is to the left or right of the line.
 *        if `return` < 0, point is to the left side
 *        if `return` >= 0, point is to the right side
 *        for more information to see
 *        https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
 * @param p
 * @param v0
 * @param v1
 * @return int
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-07
 * @copyright Copyright (c) 2024
 */
int vec2_edge(vec2i p, vec2i v0, vec2i v1) {
  return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

vec3f vec3_new(float x, float y, float z) {
    vec3f v = {x, y, z};
    return v;
}

/*
                  |                    
                 /|\ ->  
                  |  C
                  |                   ->              
                  |________________\_ b
                 /  ->             /
                /   a
              \/_
*/
vec3f vec3_cross(vec3f a, vec3f b) {
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;

    return vec3_new(x, y, z);
}

float vec3_dot(vec3f a, vec3f b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* 
    -> ->   -> 
    a - b = ba 
*/
vec3f vec3_sub(vec3f a, vec3f b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;

    return vec3_new(x, y, z);
}

vec3f vec3_normalize(vec3f a) {
    return vec3_div(a, vec3_len(a));
}

vec3f vec3_mult(vec3f a, float factor) {
    return vec3_new(a.x * factor, a.y * factor, a.z * factor);
}

float vec3_len(vec3f a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3f vec3_div(vec3f a, float divisor) {
    return vec3_mult(a, 1/divisor);
}

vec2i vec2_mult(vec2i p, float factor) {
    return vec2_new(p.x * factor, p.y * factor);
}

vec2i vec2_add(vec2i a, vec2i b) {
    return vec2_new(a.x + b.x, a.y + b.y);
}

vec3f vec3_add(vec3f a, vec3f b) {
    return vec3_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3f vec3_add3(vec3f a, vec3f b, vec3f c) {
    return vec3_new(a.x + b.x + c.x, a.y + b.y + c.y, a.z + b.z + c.z);
}

/**
 * @brief compute barycentric coordinates from Cartesian coordinates.
 * @ref   https://en.wikipedia.org/wiki/Barycentric_coordinate_system Edge approach 
 *        from chapter Conversion between barycentric and Cartesian coordinates.
 * @param p
 * @param v1
 * @param v2
 * @param v3
 * @return vec3f
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-07
 * @copyright Copyright (c) 2024
 */
vec3f barycentric(vec2i p, vec2i v1, vec2i v2, vec2i v3) {
    float lambda1 = (float)((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) \
                    / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    
    float lambda2 = (float)((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) \
                    / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    
    float lambda3 = 1 - lambda1 - lambda2;
    
    vec3f f = vec3_new(lambda1, lambda2, lambda3);
    return f;
}

/******************************************************************************
 * @brief The interpolation formula for perspective correction is as follows:
 *        I =  ((α/Z1)*I1 + (β/Z2)*I2+ (γ/Z3)*I3)*Zt
 *        and Zt = 1/(α/Z1 + β/Z2+ γ/Z3).
 *        I1、I2、I3 are the attributes values, Z1、Z2、Z3 are original z value
 *        in model file, α、β、γ are barycentric value.
 * 
 * @ref https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
 * @param v 
 * @param weight 
 * @param w 
 * @return vec3f 
 * @file maths.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-07-30
 * @copyright Copyright (c) 2024
******************************************************************************/
vec3f perspective_correct_interp(vec3f v[3], vec3f weight, vec3f w) {
    vec3f recip_w = vec3_new(1/w.x, 1/w.y, 1/w.z);
    float alpha = weight.x * recip_w.x;
    float beta  = weight.y * recip_w.y;
    float gamma = weight.z * recip_w.z;
    float Zt = 1 / (alpha + beta + gamma);
    vec3f _It = vec3_add3(vec3_mult(v[0] ,alpha),
                         vec3_mult(v[1] ,beta), 
                         vec3_mult(v[2] ,gamma));
    vec3f It = vec3_mult(_It, Zt);

    return It;
}

mat4f mat4_identity() {
    mat4f m = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    return m;
}

/**
 * @brief perspective projection matrix as follows:
 *        2n/(r-l)         0            -(r+l)/(r-l)           0
 *        0             2n/(t-b)        -(t+b)/(t-b)           0
 *        0              0               (n+f)/(n-f)      -(2nf)/(n-f)
 *        0              0                    1                0
 * notes: n and f are negative values.
 * @ref  https://www.bilibili.com/video/BV1LS4y1b7xZ/?p=1&t=0&spm_id_from=..0.0&vd_source=9f9eb54cdac68c9a0eb1a034b61df5d7
 *       https://www.bilibili.com/video/BV1X7411F744?p=4
 *       https://www.bilibili.com/video/BV1UX4y1k7CT/
 * @param fovY the camera's vertical field of view(unit in degress, conversion to radians internally).
 * @param aspect The ratio of image width to image height(width/height).
 * @param near the distance of near clipping plane, is positive value.
 * @param far the distance of far clipping plane, is positive value.
 * @return mat4f
 * @file maths.c
 * @version 0.5
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-18
 * @copyright Copyright (c) 2024
 */ 
mat4f mat4_perspective(float fovY, float aspect, float near, float far) {
    
    float radians = degrees_2_radians(fovY);
    float top = (float)tan(radians / 2) * near;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    near = -near;
    far = -far;

    mat4f Persp2Ortho = mat4_identity();

    Persp2Ortho.m[0][0] = near;
    Persp2Ortho.m[1][1] = near;
    Persp2Ortho.m[2][2] = near + far;
    Persp2Ortho.m[2][3] = - near * far;
    Persp2Ortho.m[3][2] = 1;
    Persp2Ortho.m[3][3] = 0;

    mat4f ortho = mat4_ortho(left, right, bottom, top, near, far);

    mat4f m = mat4_mult_mat4(ortho, Persp2Ortho);

    return m;
}

/******************************************************************************
 * @brief orthographic projection matrix as follows:
 *         2/(r-l)      0         0        -(r=l)/(r-l)
 *           0       2/(t-b)      0        -(t+b)/(t-b)
 *           0          0       2/(n-f)    -(n+f)/(n-f)
 *           0          0         0              1
 * notes: n and f are negative values.
 * @ref  https://www.bilibili.com/video/BV1X7411F744?p=4
 *       https://www.bilibili.com/video/BV1UX4y1k7CT/
 * @param left the negative x-axis coordinate point of near clipping plane.
 * @param right the positive x-axis coordinate point of near clipping plane.
 * @param bottom the negative y-axis coordinate point of near clipping plane.
 * @param top the positive x-axis coordinate point of near clipping plane.
 * @param near the negative z-axis coordinate point of near clipping plane.
 * @param far the negative z-axis coordinate point of far clipping plane.
 * @return mat4f 
 * @file maths.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
******************************************************************************/
mat4f mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    
    float translateX = - (right + left) / 2;
    float translateY = - (top + bottom) / 2;
    float translateZ = - (near + far) / 2;
    mat4f translate = mat4_translate(translateX, translateY, translateZ); 
    
    float scaleX = 2 / (right - left);
    float scaleY = 2 / (top - bottom);
    float scaleZ = 2 / (near - far);
    mat4f scale = mat4_scale(scaleX, scaleY, scaleZ);

    mat4f m = mat4_mult_mat4(scale, translate);

    return m;
}

mat4f mat4_zero() {
    mat4f m = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    };
    return m;
}

mat4f mat4_mult_mat4(mat4f a, mat4f b) {
    
    mat4f m = mat4_zero();
    for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 4; j++) {
          m.m[i][j] = a.m[i][0] * b.m[0][j] + \
                      a.m[i][1] * b.m[1][j] + \
                      a.m[i][2] * b.m[2][j] + \
                      a.m[i][3] * b.m[3][j];  
       }
    }
    return m;
}

vec4f vec4_new(float x, float y, float z, float w) {
    vec4f v = {x, y, z, w};
    return v;
}

vec4f mat4_mult_vec4(mat4f a, vec4f b) {
    float v[4] = {0};
    for (int i = 0; i < 4; i++) {
        v[i] = a.m[i][0] * b.x + \
               a.m[i][1] * b.y + \
               a.m[i][2] * b.z + \
               a.m[i][3] * b.w;
    }
    return vec4_new(v[0], v[1], v[2], v[3]);
}

vec4f vec3_2_vec4(vec3f a, float w) {
    return vec4_new(a.x, a.y, a.z, w);
}

vec3f vec4_2_vec3(vec4f a, float w) {
    return vec3_new(a.x/w, a.y/w, a.z/w);
}

/**
 * @brief translation matrix as follows:
 *        1 0 0 x
 *        0 1 0 y
 *        0 0 1 z
 *        0 0 0 1
 *
 * @ref https://docs.gl/gl2/glTranslate
 * @param x the x coordinates of a translation vector.
 * @param y the y coordinates of a translation vector.
 * @param z the z coordinates of a translation vector.
 * @return mat4f
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-22
 * @copyright Copyright (c) 2024
 */
mat4f mat4_translate(float x, float y, float z) {
    mat4f m= mat4_identity();
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
    return m;
}

/**
 * @brief scaling matrix as follows:
 *        x 0 0 0
 *        0 y 0 0
 *        0 0 z 0
 *        0 0 0 1
 *
 * @ref https://docs.gl/gl2/glScale
 * @param x scale factors along the x axes
 * @param y scale factors along the y axes
 * @param z scale factors along the z axes
 * @return mat4f
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-22
 * @copyright Copyright (c) 2024
 */
mat4f mat4_scale(float x, float y, float z) {
    mat4f m = mat4_identity();
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

/**
 * @brief rotate matrix(x-axis) as follows:
 *        1  0     0   0
 *        0 cosA -sinA 0
 *        0 sinA cosA  0
 *        0  0    0    1
 * @ref http://www.songho.ca/opengl/gl_anglestoaxes.html
 * @param angle unit in degress, conversion to radians internally
 * @return mat4f
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-23
 * @copyright Copyright (c) 2024
 */
mat4f mat4_rotate_x(float angle) {
    float radians = degrees_2_radians(angle);
    float cosA = cos(radians);
    float sinA = sin(radians);
    
    mat4f m = mat4_identity();
    m.m[1][1] = cosA;
    m.m[1][2] = -sinA;
    m.m[2][1] = sinA;
    m.m[2][2] = cosA;

    return m;
}

/**
 * @brief rotate matrix(y-axis) as follows:
 *        cosB  0 sinB   0
 *          0   1   0    0
 *        -sinB 0 cosB   0
 *          0   0   0    1
 * @ref http://www.songho.ca/opengl/gl_anglestoaxes.html
 * @param angle unit in degress, conversion to radians internally
 * @return mat4f
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-23
 * @copyright Copyright (c) 2024
 */
mat4f mat4_rotate_y(float angle) {
    float radians = degrees_2_radians(angle);
    float cosB = cos(radians);
    float sinB = sin(radians);
    
    mat4f m = mat4_identity();
    m.m[0][0] = cosB;
    m.m[0][2] = sinB;
    m.m[2][0] = -sinB;
    m.m[2][2] = cosB;

    return m;
}

/**
 * @brief rotate matrix(z-axis) as follows:
 *        cosC  -sinC  0   0
 *        sinC   cosC  0   0
 *         0      0    1   0
 *         0      0    0   1
 * @ref http://www.songho.ca/opengl/gl_anglestoaxes.html
 * @param angle unit in degress, conversion to radians internally
 * @return mat4f
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-23
 * @copyright Copyright (c) 2024
 */
mat4f mat4_rotate_z(float angle) {
    float radians = degrees_2_radians(angle);
    float cosC = cos(radians);
    float sinC = sin(radians);
    
    mat4f m = mat4_identity();
    m.m[0][0] = cosC;
    m.m[0][1] = -sinC;
    m.m[1][0] = sinC;
    m.m[1][1] = cosC;

    return m;
}

void vec2_print(vec2i p) {
    printf("vec2i: (%f, %f)\n", p.x, p.y);
}

void vec3_print(vec3f p) {
    printf("vec3f: (%f, %f, %f)\n", p.x, p.y, p.z);
}

void vec4_print(vec4f p) {
    printf("vec4f: (%f, %f, %f, %f)\n", p.x, p.y, p.z, p.w);
}

void vec2_fprint(FILE* opf, vec2i p) {
    fprintf(opf, "vec2i: (%f, %f)\n", p.x, p.y);
}

void vec3_fprint(FILE* opf, vec3f p) {
    fprintf(opf, "vec3f: (%f, %f, %f)\n", p.x, p.y, p.z);
}

void vec4_fprint(FILE* opf, vec4f p) {
    fprintf(opf, "vec4f: (%f, %f, %f, %f)\n", p.x, p.y, p.z, p.w);
}

/******************************************************************************
 * @brief camera is defined in the right-hand coordinate system, 
 *        and camera is always looking at the -z axis. thus 
 *        z-axis = eye - target;
 *        x-axis = up x z-axis;
 *        y-axis = z-axis x x-axis; 
 *        which `x` is cross product and `-` is vector subtraction, and
 *        x-axis、y-axis and z-axis should be normalize.
 *        so look-at matrix as follows: 
 *        x_axis.x  x_axis.y  x_axis.z  -dot(x_axis,eye)
 *        y_axis.x  y_axis.y  y_axis.z  -dot(y_axis,eye)
 *        z_axis.x  z_axis.y  z_axis.z  -dot(z_axis,eye)
 *           0         0         0                 1
 * @ref https://www.bilibili.com/video/BV1BP411Q7ej/
 *      https://www.bilibili.com/video/BV1X7411F744?p=4
 * @param eye the position of the eye point
 * @param target the position of the target point
 * @param up the position of the up vector
 * @return mat4f 
 * @file maths.c
 * @version 1.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-07-15
 * @copyright Copyright (c) 2024
******************************************************************************/
mat4f mat4_look_at(vec3f eye, vec3f target, vec3f up) {
    vec3f z_axis = vec3_normalize(vec3_sub(eye, target));
    vec3f x_axis = vec3_normalize(vec3_cross(up, z_axis));
    vec3f y_axis = vec3_cross(z_axis, x_axis);
    mat4f m = mat4_identity();

    m.m[0][0] = x_axis.x;
    m.m[0][1] = x_axis.y;
    m.m[0][2] = x_axis.z;

    m.m[1][0] = y_axis.x;
    m.m[1][1] = y_axis.y;
    m.m[1][2] = y_axis.z;

    m.m[2][0] = z_axis.x;
    m.m[2][1] = z_axis.y;
    m.m[2][2] = z_axis.z;

    m.m[0][3] = -vec3_dot(x_axis, eye);
    m.m[1][3] = -vec3_dot(y_axis, eye);
    m.m[2][3] = -vec3_dot(z_axis, eye);

    return m;
}


/******************************************************************************
 * @brief view transformation matrix as follows:
 *         width/2       0        0        width/2
 *            0       height/2    0        height/2
 *            0           0       1           0
 *            0           0       0           1 
 * 
 * @ref https://www.cnblogs.com/Gr-blogs/p/17741819.html#%E8%A7%86%E5%8F%A3%E5%8F%98%E6%8D%A2
 * @param width 
 * @param height 
 * @return mat4f 
 * @file maths.c
 * @version 1.0
 * @author Jelly (wugd827@163.com)
 * @date 2024-07-29
 * @copyright Copyright (c) 2024
******************************************************************************/
mat4f mat4_viewport(float width, float height) {
    mat4f m = mat4_identity();
    m.m[0][0] = width / 2;
    m.m[0][3] = width / 2;
    m.m[1][3] = height / 2;
    m.m[1][1] = height / 2;

    return m;
}