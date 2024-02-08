#include <stdio.h>
#include "../core/api.h"

int main() {
  vec3f t = vec3f_new(1,2,3);
  printf("%f %f %f\n", vec3f_normalize(t).x, vec3f_normalize(t).y, vec3f_normalize(t).z);
  return 0;
}
