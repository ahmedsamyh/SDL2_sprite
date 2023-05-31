#ifndef _VEC_H_
#define _VEC_H_

typedef struct {
  union {
    float x;
    float u;
  };
  union {
    float y;
    float v;
  };
} Vec2f;

#define Vec2f_fmt "(%.2f, %.2f)"
#define Vec2f_arg(v2f) v2f.x, v2f.y

Vec2f Vec2f_make(float x, float y);
Vec2f Vec2f_add(Vec2f v1, Vec2f v2);
Vec2f Vec2f_adds(Vec2f v1, float num);
Vec2f Vec2f_sub(Vec2f v1, Vec2f v2);
Vec2f Vec2f_subs(Vec2f v1, float num);
Vec2f Vec2f_mult(Vec2f v1, Vec2f v2);
Vec2f Vec2f_mults(Vec2f v1, float num);
Vec2f Vec2f_div(Vec2f v1, Vec2f v2);
Vec2f Vec2f_divs(Vec2f v1, float num);
float Vec2f_mag(const Vec2f v);
float Vec2f_magSq(const Vec2f v);

#endif /* _VEC_H_ */

#ifdef VEC_IMPLEMENTATION

Vec2f Vec2f_make(float x, float y) { return (Vec2f){x, y}; }
Vec2f Vec2f_add(Vec2f v1, Vec2f v2) {
  return (Vec2f){v1.x + v2.x, v1.y + v2.y};
}
Vec2f Vec2f_adds(Vec2f v1, float num) {
  return (Vec2f){v1.x + num, v1.y + num};
}
Vec2f Vec2f_sub(Vec2f v1, Vec2f v2) {
  return (Vec2f){v1.x - v2.x, v1.y - v2.y};
}
Vec2f Vec2f_subs(Vec2f v1, float num) {
  return (Vec2f){v1.x - num, v1.y - num};
}
Vec2f Vec2f_mult(Vec2f v1, Vec2f v2) {
  return (Vec2f){v1.x * v2.x, v1.y * v2.y};
}
Vec2f Vec2f_mults(Vec2f v1, float num) {
  return (Vec2f){v1.x * num, v1.y * num};
}
Vec2f Vec2f_div(Vec2f v1, Vec2f v2) {
  return (Vec2f){v1.x / v2.x, v1.y / v2.y};
}
Vec2f Vec2f_divs(Vec2f v1, float num) {
  return (Vec2f){v1.x / num, v1.y / num};
}
float Vec2f_mag(const Vec2f v) { return sqrtf((v.x * v.x) + (v.y * v.y)); }
float Vec2f_magSq(const Vec2f v) { return ((v.x * v.x) + (v.y * v.y)); }

#endif
