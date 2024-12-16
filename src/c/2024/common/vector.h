#pragma once

typedef struct {
  int x;
  int y;
} V2;

#define v2_fmt "%i,%i"
#define v2_arg(v) (v).x, (v).y

inline V2 v2_add(V2 a, V2 b) {
  return (V2){a.x + b.x, a.y + b.y};
}
inline V2 v2_sub(V2 a, V2 b) {
  return (V2){a.x - b.x, a.y - b.y};
}
inline V2 v2_scale(V2 v, int factor) {
  return (V2){v.x*factor, v.y*factor};
}
inline V2 v2_rotate(V2 v) {
  return (V2){-v.y, v.x};
}
