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

#ifdef TML_STATIC
    #define TMLDEF static inline
#else
    #define TMLDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

TMLDEF float tml_deg2rad(float angle);

TMLDEF vec2 tml_vec2_zero();
TMLDEF vec2 tml_vec2_add(vec2 a, vec2 b);
TMLDEF vec2 tml_vec2_sub(vec2 a, vec2 b);
TMLDEF vec2 tml_vec2_mul(vec2 a, vec2 b);
TMLDEF vec2 tml_vec2_div(vec2 a, vec2 b);
TMLDEF float tml_vec2_magnitude(vec2 v);
TMLDEF vec2 tml_vec2_normalize(vec2 v);
TMLDEF float tml_vec2_dot(vec2 a, vec2 b);

TMLDEF vec3 tml_vec3_zero();
TMLDEF vec3 tml_vec3_add(vec3 a, vec3 b);
TMLDEF vec3 tml_vec3_sub(vec3 a, vec3 b);
TMLDEF vec3 tml_vec3_mul(vec3 a, vec3 b);
TMLDEF vec3 tml_vec3_div(vec3 a, vec3 b);
TMLDEF float tml_vec3_magnitude(vec3 v);
TMLDEF vec3 tml_vec3_normalize(vec3 v);
TMLDEF float tml_vec3_dot(vec3 a, vec3 b);

TMLDEF const matrix tml_mat_identity();

TMLDEF matrix tml_mat_translate(vec3 position);
TMLDEF matrix tml_mat_rotatex(float angle);
TMLDEF matrix tml_mat_rotatey(float angle);
TMLDEF matrix tml_mat_rotatez(float angle);
TMLDEF matrix tml_mat_scale(vec3 amount);

TMLDEF matrix tml_mat_mul(matrix left, matrix right);
TMLDEF matrix tml_mat_model(vec3 pos, vec3 rotation, vec3 scale);
TMLDEF matrix tml_mat_view(vec3 position, float rotation, float zoom);

TMLDEF matrix tml_mat_ortho(float left, float right, float top, float bottom, float far, float near);

#ifdef TML_IMPL

TMLDEF float tml_deg2rad(float angle){
    return angle * (PI / 180.0f);
}

TMLDEF vec2 tml_vec2_zero(){
    return (vec2){0.0f, 0.0f};
}

TMLDEF vec2 tml_vec2_add(vec2 a, vec2 b){
    return (vec2){
        a.x+b.x,
        a.y+b.y
    };
}

TMLDEF vec2 tml_vec2_sub(vec2 a, vec2 b){
    return (vec2){
        a.x-b.x,
        a.y-b.y
    };
}
TMLDEF vec2 tml_vec2_mul(vec2 a, vec2 b){
    return (vec2){
        a.x*b.x,
        a.y*b.y
    };
}
TMLDEF vec2 tml_vec2_div(vec2 a, vec2 b){
    return (vec2){
        a.x/b.x,
        a.y/b.y
    };
}

TMLDEF float tml_vec2_magnitude(vec2 v){
    return hypot(v.x, v.y);
}
TMLDEF vec2 tml_vec2_normalize(vec2 v){
    float len = tml_vec2_magnitude(v);

    if (len == 0) {
        return tml_vec2_zero();
    }

    return (vec2){
        v.x / len,
        v.y / len
    };
}

TMLDEF float tml_vec2_dot(vec2 a, vec2 b){
    return (a.x*b.x) + (a.y*b.y);
}

TMLDEF vec3 tml_vec3_zero(){
    return (vec3){0.0f, 0.0f, 0.0f};
}

TMLDEF vec3 tml_vec3_add(vec3 a, vec3 b){
    return (vec3){
        a.x+b.x,
        a.y+b.y,
        a.z+b.z
    };
}

TMLDEF vec3 tml_vec3_sub(vec3 a, vec3 b){
    return (vec3){
        a.x-b.x,
        a.y-b.y,
        a.z-b.z
    };
}

TMLDEF vec3 tml_vec3_mul(vec3 a, vec3 b){
    return (vec3){
        a.x*b.x,
        a.y*b.y,
        a.z*b.z
    };
}

TMLDEF vec3 tml_vec3_div(vec3 a, vec3 b){
    return (vec3){
        a.x/b.x,
        a.y/b.y,
        a.z/b.z
    };
}

TMLDEF float tml_vec3_magnitude(vec3 v){
    return hypot(hypot(v.x, v.y), v.z);
}

TMLDEF vec3 tml_vec3_normalize(vec3 v){
    float len = tml_vec3_magnitude(v);

    if (len == 0) {
        return tml_vec3_zero();
    }

    return (vec3){
        v.x / len,
        v.y / len,
        v.z / len
    };
}

TMLDEF float tml_vec3_dot(vec3 a, vec3 b){
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

TMLDEF const matrix tml_mat_identity(){
    matrix result;

    result.m[0][0] = 1; result.m[1][0] = 0; result.m[2][0] = 0; result.m[3][0] = 0;
    result.m[0][1] = 0; result.m[1][1] = 1; result.m[2][1] = 0; result.m[3][1] = 0;
    result.m[0][2] = 0; result.m[1][2] = 0; result.m[2][2] = 1; result.m[3][2] = 0;
    result.m[0][3] = 0; result.m[1][3] = 0; result.m[2][3] = 0; result.m[3][3] = 1;

    return result;
}


TMLDEF matrix tml_mat_translate(vec3 position) {
    matrix result = tml_mat_identity();

    result.m[3][0] = position.x;
    result.m[3][1] = position.y;
    result.m[3][2] = position.z;

    return result;
}

TMLDEF matrix tml_mat_rotatex(float angle){
    matrix result = tml_mat_identity();

    float rads = tml_deg2rad(angle);

    float cosv = cosf(rads);
    float sinv = sinf(rads);

    result.m[1][1] = cosv;
    result.m[2][1] = -sinv;

    result.m[1][2] = sinv;
    result.m[2][2] = cosv;

    return result;
}

TMLDEF matrix tml_mat_rotatey(float angle){
    matrix result = tml_mat_identity();

    float rads = tml_deg2rad(angle);

    float cosv = cosf(rads);
    float sinv = sinf(rads);

    result.m[0][0] = cosv;
    result.m[2][0] = sinv;

    result.m[0][2] = -sinv;
    result.m[2][2] = cosv;

    return result;
}

TMLDEF matrix tml_mat_rotatez(float angle){
    matrix result = tml_mat_identity();

    float rads = tml_deg2rad(angle);

    float cosv = cosf(rads);
    float sinv = sinf(rads);

    result.m[0][0] = cosv;
    result.m[1][0] = -sinv;

    result.m[0][1] = sinv;
    result.m[1][1] = cosv;


    return result;
}

TMLDEF matrix tml_mat_scale(vec3 amount){
    matrix result = tml_mat_identity();

    result.m[0][0] = amount.x;
    result.m[1][1] = amount.y;
    result.m[2][2] = amount.z;

    return result;
}

TMLDEF matrix tml_mat_mul(matrix left, matrix right){
    matrix result = tml_mat_identity();

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

TMLDEF matrix tml_mat_model(vec3 pos, vec3 rotation, vec3 scale){
    matrix result = tml_mat_identity();

    matrix t = tml_mat_identity();
    matrix rx = tml_mat_identity();
    matrix ry = tml_mat_identity();
    matrix rz = tml_mat_identity();
    matrix s = tml_mat_identity();

    t = tml_mat_translate(pos);
    rx = tml_mat_rotatex(rotation.x);
    ry = tml_mat_rotatey(rotation.y);
    rz = tml_mat_rotatez(rotation.z);

    matrix r = tml_mat_mul(tml_mat_mul(rx, ry), rz);

    s = tml_mat_scale(scale);

    result = tml_mat_mul(tml_mat_mul(t, r), s);

    return result;
}

TMLDEF matrix tml_mat_view(vec3 position, float rotation, float zoom){
    matrix t = tml_mat_translate((vec3){
        -position.x,
        -position.y,
        -position.z
    });
    matrix r = tml_mat_rotatez(-rotation);
    matrix s = tml_mat_scale((vec3){zoom, zoom, zoom});

    return tml_mat_mul(tml_mat_mul(t, r), s);
}

TMLDEF matrix tml_mat_ortho(float left, float right, float top, float bottom, float far, float near){
    matrix result = tml_mat_identity();

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
