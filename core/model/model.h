#ifndef _MODEL_H
#define _MODEL_H

#include "../common.h"
#include "../utils/utils.h"
#include "../image/image.h"
#include "../maths/maths.h"

typedef struct {
  vec4f vertex[3]; 
  vec3f uv[3];
  vec3f normal[3];
}model_t;



#endif