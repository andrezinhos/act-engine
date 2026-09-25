#pragma once
#include <math.h>

#define PI 3.14159265358979323846f

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct {
    float x, y, z, w;
} Vec4;

typedef struct {
    float v[4][4];
} Matrix;

struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
};

struct Camera2D{
    Vec2 position;
    float rotation;
    float zoom;
};

struct Camera3D{
    Vec3 position;
    Vec3 target;
    Vec3 up;
    float fovy;
};

#ifdef __cplusplus
extern "C" {
#endif

inline Vec2 Vec2Zero(){
    return (Vec2){0.0f, 0.0f};
}

inline Vec2 Vec2One(){
    return (Vec2){1.0f, 1.0f};
}

inline Vec2 AddVec2(Vec2 v1, Vec2 v2){
    return (Vec2){v1.x + v2.x, v1.y + v2.y};
}

inline Vec2 SubtractVec2(Vec2 v1, Vec2 v2){
    return (Vec2){v1.x - v2.x, v1.y - v2.y};
}


inline Vec3 Vec3Zero(){
    return (Vec3){0.0f, 0.0f, 0.0f};
}

inline Vec3 Vec3One(){
    return (Vec3){1.0f, 1.0f, 1.0f};
}

inline Vec3 AddVec3(Vec3 v1, Vec3 v2){
    return (Vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline Vec3 SubtractVec3(Vec3 v1, Vec3 v2){
    return (Vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline const Matrix MatrixIdentity() {
    Matrix result = {};

    result.v[0][0] = 1; result.v[1][0] = 0; result.v[2][0] = 0; result.v[3][0] = 0;
    result.v[0][1] = 0; result.v[1][1] = 1; result.v[2][1] = 0; result.v[3][1] = 0;
    result.v[0][2] = 0; result.v[1][2] = 0; result.v[2][2] = 1; result.v[3][2] = 0;
    result.v[0][3] = 0; result.v[1][3] = 0; result.v[2][3] = 0; result.v[3][3] = 1;

    return result;
}


inline float Rads(float angle){
    return angle * PI / 180.0f;
}

inline float Clamp(float value, float min, float max){
    float result;
    if (value < min) result = min;
    else result = value;

    if (result > max) result = max;

    return result;
}

inline float Lerp(float start, float end, float amount){
    float result = start + amount * (end - start);
    return result;
}

inline float Normalize(float value, float start, float end){
    float result = (value - start) / (end - start);
    return result;
}

// finds the magnitude of a Vec2
inline float Vec2Length(Vec2 v){
    return hypot(v.x, v.y);
}

inline float Vec3Length(Vec3 v){
    return hypot(v.x, v.y, v.z);
}

inline Vec2 Vec2Normalize(Vec2 v){
    float len = Vec2Length(v);
    if (len == 0) return Vec2Zero();
    return (Vec2){v.x / len, v.y / len};
}

inline Vec3 Vec3Normalize(Vec3 v){
    float len = Vec3Length(v);
    if (len == 0) return Vec3Zero();
    return (Vec3){v.x / len, v.y / len, v.z / len};
}

inline Matrix TranslateMatrix(Vec3 position){
    Matrix result = MatrixIdentity();

    result.v[3][0] = position.x; // m12
    result.v[3][1] = position.y; // m13
    result.v[3][2] = position.z; // m14

    return result;
}

inline Matrix MatrixRotateX(float angle){
    Matrix result = MatrixIdentity();

    float rad = Rads(angle);

    float cos = cosf(rad);
    float sin = sinf(rad);

    result.v[1][1] = cos; // m5
    result.v[2][1] = -sin; // m9
    result.v[1][2] = sin; // m6
    result.v[2][2] = cos; // m10

    return result;
}

inline Matrix MatrixRotateY(float angle){
    Matrix result = MatrixIdentity();

    float rad = Rads(angle);

    float cos = cosf(rad);
    float sin = sinf(rad);

    result.v[0][0] = cos; // m0
    result.v[2][0] = sin; // m8
    result.v[0][2] = -sin; // m2
    result.v[2][2] = cos; // m10

    return result;
}

inline Matrix MatrixRotateZ(float angle){
    Matrix result = MatrixIdentity();

    float rad = Rads(angle);

    float cos = cosf(rad);
    float sin = sinf(rad);

    result.v[0][0] = cos; // m0
    result.v[1][0] = -sin; // m4
    result.v[0][1] = sin; // m1
    result.v[1][1] = cos; // m5

    return result;
}

inline Matrix ScaleMatrix(Vec3 scale){
    Matrix result = MatrixIdentity();

    result.v[0][0] = scale.x;
    result.v[1][1] = scale.y;
    result.v[2][2] = scale.z;

    return result;
}

inline Vec3 Cross(Vec3 v1, Vec3 v2){
    return (Vec3){
        (v1.y*v2.z) - (v1.z*v2.y),
        (v1.z*v2.x) - (v1.x*v2.z),
        (v1.x*v2.y) - (v1.y*v2.x)
    };
}

inline float Vec2Dot(Vec2 v1, Vec2 v2){
    return (float)((v1.x*v2.x)+(v1.y*v2.y));
}

inline float Vec3Dot(Vec3 v1, Vec3 v2){
    return (float)((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z));
}

// the result is: the rows of the left one, by the cols of the right one
inline Matrix MultiplyMatrix(Matrix left, Matrix right){
    Matrix result = MatrixIdentity();

    // column 0
    result.v[0][0] =
        left.v[0][0]*right.v[0][0] +
        left.v[1][0]*right.v[0][1] +
        left.v[2][0]*right.v[0][2] +
        left.v[3][0]*right.v[0][3];

    result.v[0][1] =
        left.v[0][1]*right.v[0][0] +
        left.v[1][1]*right.v[0][1] +
        left.v[2][1]*right.v[0][2] +
        left.v[3][1]*right.v[0][3];

    result.v[0][2] =
        left.v[0][2]*right.v[0][0] +
        left.v[1][2]*right.v[0][1] +
        left.v[2][2]*right.v[0][2] +
        left.v[3][2]*right.v[0][3];

    result.v[0][3] =
        left.v[0][3]*right.v[0][0] +
        left.v[1][3]*right.v[0][1] +
        left.v[2][3]*right.v[0][2] +
        left.v[3][3]*right.v[0][3]; // m3

    // column 1

    result.v[1][0] =
        left.v[0][0]*right.v[1][0] +
        left.v[1][0]*right.v[1][1] +
        left.v[2][0]*right.v[1][2] +
        left.v[3][0]*right.v[1][3];

    result.v[1][1] =
        left.v[0][1]*right.v[1][0] +
        left.v[1][1]*right.v[1][1] +
        left.v[2][1]*right.v[1][2] +
        left.v[3][1]*right.v[1][3];

    result.v[1][2] =
        left.v[0][2]*right.v[1][0] +
        left.v[1][2]*right.v[1][1] +
        left.v[2][2]*right.v[1][2] +
        left.v[3][2]*right.v[1][3];

    result.v[1][3] =
        left.v[0][3]*right.v[1][0] +
        left.v[1][3]*right.v[1][1] +
        left.v[2][3]*right.v[1][2] +
        left.v[3][3]*right.v[1][3]; // m7

    // column 2

    result.v[2][0] =
        left.v[0][0]*right.v[2][0] +
        left.v[1][0]*right.v[2][1] +
        left.v[2][0]*right.v[2][2] +
        left.v[3][0]*right.v[2][3];

    result.v[2][1] =
        left.v[0][1]*right.v[2][0] +
        left.v[1][1]*right.v[2][1] +
        left.v[2][1]*right.v[2][2] +
        left.v[3][1]*right.v[2][3];

    result.v[2][2] =
        left.v[0][2]*right.v[2][0] +
        left.v[1][2]*right.v[2][1] +
        left.v[2][2]*right.v[2][2] +
        left.v[3][2]*right.v[2][3];

    result.v[2][3] =
        left.v[0][3]*right.v[2][0] +
        left.v[1][3]*right.v[2][1] +
        left.v[2][3]*right.v[2][2] +
        left.v[3][3]*right.v[2][3]; // m11

    // column 3

    result.v[3][0] =
        left.v[0][0]*right.v[3][0] +
        left.v[1][0]*right.v[3][1] +
        left.v[2][0]*right.v[3][2] +
        left.v[3][0]*right.v[3][3];

    result.v[3][1] =
        left.v[0][1]*right.v[3][0] +
        left.v[1][1]*right.v[3][1] +
        left.v[2][1]*right.v[3][2] +
        left.v[3][1]*right.v[3][3];

    result.v[3][2] =
        left.v[0][2]*right.v[3][0] +
        left.v[1][2]*right.v[3][1] +
        left.v[2][2]*right.v[3][2] +
        left.v[3][2]*right.v[3][3];

    result.v[3][3] =
        left.v[0][3]*right.v[3][0] +
        left.v[1][3]*right.v[3][1] +
        left.v[2][3]*right.v[3][2] +
        left.v[3][3]*right.v[3][3]; // m15

    return result;
}

inline Matrix GetMatrixModel(Vec3& position, Vec3& rotation, Vec3& scale){
    Matrix t = TranslateMatrix(position);
    Matrix rx = MatrixRotateX(rotation.x);
    Matrix ry = MatrixRotateY(rotation.y);
    Matrix rz = MatrixRotateZ(rotation.z);

    Matrix r = MultiplyMatrix(MultiplyMatrix(rx, ry), rz);
    Matrix s = ScaleMatrix(scale);

    return MultiplyMatrix(MultiplyMatrix(t, r), s);
}

inline Matrix LookAt(Vec3 eye, Vec3 target, Vec3 upChute){
    Vec3 forward = Vec3Normalize(SubtractVec3(target, eye));
    Vec3 side = Vec3Normalize(Cross(forward, upChute));
    Vec3 up = Vec3Normalize(Cross(side, forward));

    Matrix result = MatrixIdentity();

    result.v[0][0] = side.x; result.v[1][0] = side.y; result.v[2][0] = side.z;
    result.v[0][1] = up.x; result.v[1][1] = up.y; result.v[2][1] = up.z;
    result.v[0][2] = -forward.x; result.v[1][2] = -forward.y; result.v[2][2] = -forward.z;

    result.v[3][0] = -Vec3Dot(side, eye);
    result.v[3][1] = -Vec3Dot(up, eye);
    result.v[3][2] = Vec3Dot(forward, eye);

    return result;
}

inline Matrix OrthoMatrix(float left, float right, float top, float bottom){
    Matrix result = MatrixIdentity();

    result.v[0][0] = 2.0f / (right - left);
    result.v[1][1] = 2.0f / (top - bottom);
    result.v[2][2] = 1.0f;

    result.v[3][0] = -(right + left) / (right - left);
    result.v[3][1] = -(top + bottom) / (top - bottom);

    return result;
}

inline Matrix GetViewMatrix(Camera2D& cam){
    Matrix t = TranslateMatrix({-cam.position.x, -cam.position.y, 0.0f});
    Matrix r = MatrixRotateZ(-cam.rotation);
    Matrix s = ScaleMatrix({cam.zoom, cam.zoom, 1.0});

    return MultiplyMatrix(MultiplyMatrix(t, r), s);
}

inline Matrix GetProjectionMatrix(int width, int height){
    return OrthoMatrix(
        0.0f, (float)width,
        0.0f, (float)height
    );
}

inline Matrix PerspectiveMatrix(float fov, float aspect, float near, float far){
    float fovrad = Rads(fov);
    float f = 1.0f / tanf(fovrad * 0.5f);

    Matrix result = {};
    result.v[0][0] = f / aspect;
    result.v[1][1] = f;
    result.v[2][2] = -(far + near) / (far - near);
    result.v[2][3] = -1.0f;
    result.v[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

#ifdef __cplusplus
}
#endif
