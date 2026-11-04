#pragma once
#include <math.h>

#define PI 3.14159265358979323846f

typedef struct vec2 {
    float x;
    float y;
} vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct vec4 vec4;

typedef struct matrix{
    float m[4][4];
} matrix;

#ifdef BML_STATIC
    #define BMLDEF static inline
#else
    #define BMLDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

BMLDEF float bml_deg2rad(float angle);

BMLDEF vec2 bml_vec2_zero();
BMLDEF vec2 bml_vec2_add(vec2 a, vec2 b);
BMLDEF vec2 bml_vec2_sub(vec2 a, vec2 b);
BMLDEF vec2 bml_vec2_mul(vec2 a, vec2 b);
BMLDEF vec2 bml_vec2_div(vec2 a, vec2 b);
BMLDEF float bml_vec2_magnitude(vec2 v);
BMLDEF vec2 bml_vec2_normalize(vec2 v);
BMLDEF float bml_vec2_dot(vec2 a, vec2 b);

BMLDEF vec3 bml_vec3_zero();
BMLDEF vec3 bml_vec3_add(vec3 a, vec3 b);
BMLDEF vec3 bml_vec3_sub(vec3 a, vec3 b);
BMLDEF vec3 bml_vec3_mul(vec3 a, vec3 b);
BMLDEF vec3 bml_vec3_div(vec3 a, vec3 b);
BMLDEF float bml_vec3_magnitude(vec3 v);
BMLDEF vec3 bml_vec3_normalize(vec3 v);
BMLDEF float bml_vec3_dot(vec3 a, vec3 b);

BMLDEF matrix bml_mat_identity();

BMLDEF matrix bml_mat_translate(vec3 position);
BMLDEF matrix bml_mat_rotatex(float angle);
BMLDEF matrix bml_mat_rotatey(float angle);
BMLDEF matrix bml_mat_rotatez(float angle);
BMLDEF matrix bml_mat_scale(vec3 amount);

BMLDEF matrix bml_mat_mul(matrix left, matrix right);
BMLDEF matrix bml_mat_model(vec3 pos, vec3 rotation, vec3 scale);
BMLDEF matrix bml_mat_view(vec3 position, float rotation, float zoom);

BMLDEF matrix bml_mat_ortho(float left, float right, float top, float bottom, float far, float near);

#ifdef BML_IMPL

BMLDEF float bml_deg2rad(float angle){
    return angle * (PI / 180.0f);
}

BMLDEF vec2 bml_vec2_zero(){
    return (vec2){0.0f, 0.0f};
}

BMLDEF vec2 bml_vec2_add(vec2 a, vec2 b){
    return (vec2){
        a.x+b.x,
        a.y+b.y
    };
}

BMLDEF vec2 bml_vec2_sub(vec2 a, vec2 b){
    return (vec2){
        a.x-b.x,
        a.y-b.y
    };
}
BMLDEF vec2 bml_vec2_mul(vec2 a, vec2 b){
    return (vec2){
        a.x*b.x,
        a.y*b.y
    };
}
BMLDEF vec2 bml_vec2_div(vec2 a, vec2 b){
    return (vec2){
        a.x/b.x,
        a.y/b.y
    };
}

BMLDEF float bml_vec2_magnitude(vec2 v){
    return hypot(v.x, v.y);
}
BMLDEF vec2 bml_vec2_normalize(vec2 v){
    float len = bml_vec2_magnitude(v);

    if (len == 0) {
        return bml_vec2_zero();
    }

    return (vec2){
        v.x / len,
        v.y / len
    };
}

BMLDEF float bml_vec2_dot(vec2 a, vec2 b){
    return (a.x*b.x) + (a.y*b.y);
}

BMLDEF vec3 bml_vec3_zero(){
    return (vec3){0.0f, 0.0f, 0.0f};
}

BMLDEF vec3 bml_vec3_add(vec3 a, vec3 b){
    return (vec3){
        a.x+b.x,
        a.y+b.y,
        a.z+b.z
    };
}

BMLDEF vec3 bml_vec3_sub(vec3 a, vec3 b){
    return (vec3){
        a.x-b.x,
        a.y-b.y,
        a.z-b.z
    };
}

BMLDEF vec3 bml_vec3_mul(vec3 a, vec3 b){
    return (vec3){
        a.x*b.x,
        a.y*b.y,
        a.z*b.z
    };
}

BMLDEF vec3 bml_vec3_div(vec3 a, vec3 b){
    return (vec3){
        a.x/b.x,
        a.y/b.y,
        a.z/b.z
    };
}

BMLDEF float bml_vec3_magnitude(vec3 v){
    return hypot(hypot(v.x, v.y), v.z);
}

BMLDEF vec3 bml_vec3_normalize(vec3 v){
    float len = bml_vec3_magnitude(v);

    if (len == 0) {
        return bml_vec3_zero();
    }

    return (vec3){
        v.x / len,
        v.y / len,
        v.z / len
    };
}

BMLDEF float bml_vec3_dot(vec3 a, vec3 b){
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

BMLDEF matrix bml_mat_identity(){
    matrix result;

    result.m[0][0] = 1; result.m[1][0] = 0; result.m[2][0] = 0; result.m[3][0] = 0;
    result.m[0][1] = 0; result.m[1][1] = 1; result.m[2][1] = 0; result.m[3][1] = 0;
    result.m[0][2] = 0; result.m[1][2] = 0; result.m[2][2] = 1; result.m[3][2] = 0;
    result.m[0][3] = 0; result.m[1][3] = 0; result.m[2][3] = 0; result.m[3][3] = 1;

    return result;
}


BMLDEF matrix bml_mat_translate(vec3 position) {
    matrix result = bml_mat_identity();

    result.m[3][0] = position.x;
    result.m[3][1] = position.y;
    result.m[3][2] = position.z;

    return result;
}

BMLDEF matrix bml_mat_rotatex(float angle){
    matrix result = bml_mat_identity();

    float rads = bml_deg2rad(angle);

    float cosv = cosf(rads);
    float sinv = sinf(rads);

    result.m[1][1] = cosv;
    result.m[2][1] = -sinv;

    result.m[1][2] = sinv;
    result.m[2][2] = cosv;

    return result;
}

BMLDEF matrix bml_mat_rotatey(float angle){
    matrix result = bml_mat_identity();

    float rads = bml_deg2rad(angle);

    float cosv = cosf(rads);
    float sinv = sinf(rads);

    result.m[0][0] = cosv;
    result.m[2][0] = sinv;

    result.m[0][2] = -sinv;
    result.m[2][2] = cosv;

    return result;
}

BMLDEF matrix bml_mat_rotatez(float angle){
    matrix result = bml_mat_identity();

    float rads = bml_deg2rad(angle);

    float cosv = cosf(rads);
    float sinv = sinf(rads);

    result.m[0][0] = cosv;
    result.m[1][0] = -sinv;

    result.m[0][1] = sinv;
    result.m[1][1] = cosv;


    return result;
}

BMLDEF matrix bml_mat_scale(vec3 amount){
    matrix result = bml_mat_identity();

    result.m[0][0] = amount.x;
    result.m[1][1] = amount.y;
    result.m[2][2] = amount.z;

    return result;
}

BMLDEF matrix bml_mat_mul(matrix left, matrix right){
    matrix result = bml_mat_identity();

    result.m[0][0] =
        left.m[0][0]*right.m[0][0] +
        left.m[1][0]*right.m[0][1] +
        left.m[2][0]*right.m[0][2] +
        left.m[3][0]*right.m[0][3];

    result.m[0][1] =
        left.m[0][1]*right.m[0][0] +
        left.m[1][1]*right.m[0][1] +
        left.m[2][1]*right.m[0][2] +
        left.m[3][1]*right.m[0][3];

    result.m[0][2] =
        left.m[0][2]*right.m[0][0] +
        left.m[1][2]*right.m[0][1] +
        left.m[2][2]*right.m[0][2] +
        left.m[3][2]*right.m[0][3];

    result.m[0][3] =
        left.m[0][3]*right.m[0][0] +
        left.m[1][3]*right.m[0][1] +
        left.m[2][3]*right.m[0][2] +
        left.m[3][3]*right.m[0][3];


    result.m[1][0] =
        left.m[0][0]*right.m[1][0] +
        left.m[1][0]*right.m[1][1] +
        left.m[2][0]*right.m[1][2] +
        left.m[3][0]*right.m[1][3];

    result.m[1][1] =
        left.m[0][1]*right.m[1][0] +
        left.m[1][1]*right.m[1][1] +
        left.m[2][1]*right.m[1][2] +
        left.m[3][1]*right.m[1][3];

    result.m[1][2] =
        left.m[0][2]*right.m[1][0] +
        left.m[1][2]*right.m[1][1] +
        left.m[2][2]*right.m[1][2] +
        left.m[3][2]*right.m[1][3];

    result.m[1][3] =
        left.m[0][3]*right.m[1][0] +
        left.m[1][3]*right.m[1][1] +
        left.m[2][3]*right.m[1][2] +
        left.m[3][3]*right.m[1][3];

    result.m[2][0] =
        left.m[0][0]*right.m[2][0] +
        left.m[1][0]*right.m[2][1] +
        left.m[2][0]*right.m[2][2] +
        left.m[3][0]*right.m[2][3];

    result.m[2][1] =
        left.m[0][1]*right.m[2][0] +
        left.m[1][1]*right.m[2][1] +
        left.m[2][1]*right.m[2][2] +
        left.m[3][1]*right.m[2][3];

    result.m[2][2] =
        left.m[0][2]*right.m[2][0] +
        left.m[1][2]*right.m[2][1] +
        left.m[2][2]*right.m[2][2] +
        left.m[3][2]*right.m[2][3];

    result.m[2][3] =
        left.m[0][3]*right.m[2][0] +
        left.m[1][3]*right.m[2][1] +
        left.m[2][3]*right.m[2][2] +
        left.m[3][3]*right.m[2][3];

    result.m[3][0] =
        left.m[0][0]*right.m[3][0] +
        left.m[1][0]*right.m[3][1] +
        left.m[2][0]*right.m[3][2] +
        left.m[3][0]*right.m[3][3];

    result.m[3][1] =
        left.m[0][1]*right.m[3][0] +
        left.m[1][1]*right.m[3][1] +
        left.m[2][1]*right.m[3][2] +
        left.m[3][1]*right.m[3][3];

    result.m[3][2] =
        left.m[0][2]*right.m[3][0] +
        left.m[1][2]*right.m[3][1] +
        left.m[2][2]*right.m[3][2] +
        left.m[3][2]*right.m[3][3];

    result.m[3][3] =
        left.m[0][3]*right.m[3][0] +
        left.m[1][3]*right.m[3][1] +
        left.m[2][3]*right.m[3][2] +
        left.m[3][3]*right.m[3][3];

    return result;
}

BMLDEF matrix bml_mat_model(vec3 pos, vec3 rotation, vec3 scale){
    matrix result = bml_mat_identity();

    matrix t = bml_mat_identity();
    matrix rx = bml_mat_identity();
    matrix ry = bml_mat_identity();
    matrix rz = bml_mat_identity();
    matrix s = bml_mat_identity();

    t = bml_mat_translate(pos);
    rx = bml_mat_rotatex(rotation.x);
    ry = bml_mat_rotatey(rotation.y);
    rz = bml_mat_rotatez(rotation.z);

    matrix r = bml_mat_mul(bml_mat_mul(rx, ry), rz);

    s = bml_mat_scale(scale);

    result = bml_mat_mul(bml_mat_mul(t, r), s);

    return result;
}

BMLDEF matrix bml_mat_view(vec3 position, float rotation, float zoom){
    matrix t = bml_mat_translate((vec3){
        -position.x,
        -position.y,
        -position.z
    });
    matrix r = bml_mat_rotatez(-rotation);
    matrix s = bml_mat_scale((vec3){zoom, zoom, zoom});

    return bml_mat_mul(bml_mat_mul(t, r), s);
}

BMLDEF matrix bml_mat_ortho(float left, float right, float top, float bottom, float far, float near){
    matrix result = bml_mat_identity();

    result.m[0][0] = 2.0f / (left-right);
    result.m[1][1] = 2.0f / (top-bottom);
    result.m[2][2] = -2.0f / (far-near);

    result.m[3][0] = -(left+right) / (left-right);
    result.m[3][1] = -(top+bottom) / (top-bottom);
    result.m[3][2] = -(far+near) / (far-near);

    return result;
}

#endif

#ifdef __cplusplus
}
#endif
