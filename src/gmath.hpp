#pragma once
#include <cmath>

constexpr float PI = 3.14159265358979323846f;

struct Vec2{
    float x, y;


	Vec2() : x(0.0f), y(0.0f) {}
	
    Vec2(float x, float y) : x(x), y(y) {}

    static Vec2 Zero(){
        return {0.0f, 0.0f};
    }

    static Vec2 One(){
        return {1.0f, 1.0f};
    }

    static Vec2 AddVec2(Vec2 v1, Vec2 v2){
        Vec2 result = {v1.x + v2.x, v1.y + v2.y};
        return result;
    }

    static Vec2 SubtractVec2(Vec2 v1, Vec2 v2){
        Vec2 result = {v1.x - v2.x, v1.y - v2.y};
        return result;
    }
};

struct Vec3{
    float x, y, z;

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    static Vec3 Zero(){
        return {0.0f, 0.0f, 0.0f};
    }

    static Vec3 One(){
        return {1.0f, 1.0f, 1.0f};
    }

    static Vec3 AddVec3(Vec3 v1, Vec3 v2){
        Vec3 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
        return result;
    }

    static  Vec3 SubtractVec3(Vec3 v1, Vec3 v2){
        Vec3 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
        return result;
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    // Sobrecarga do operador -
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    // Sobrecarga do operador *
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
};

struct Vec4{
    float x, y, z, w;
};

typedef Vec4 Quaternion;

struct Matrix{
    float m[4][4];

    float& at(int col, int row){ return m[col][row]; }

    static constexpr Matrix Identity(){
        return {{
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }};
    }
};

struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
};

struct Camera2D{
    Vec2 position;
    float rotation;
    float zoom;
    Camera2D(Vec2 position, float rotation, float zoom)
        : position(position), rotation(rotation), zoom(zoom) {}
};

struct Camera3D{
    Vec3 position;
    Vec3 target;
    Vec3 up;
    float fovy;
    Camera3D(Vec3 position, Vec3 target, Vec3 up, float fovy)
        : position(position), target(target), up(up), fovy(fovy) {}
};

namespace gmath{
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
    inline float Length(Vec2 v){
        float result = std::hypot(v.x, v.y);
        return result;
    }

    inline float Length(Vec3 v){
        float result = std::hypot(v.x, v.y, v.z);
        return result;
    }

    inline Vec2 Normalize(Vec2 v){
        float len = Length(v);
        if (len == 0) return Vec2::Zero();
        return {v.x / len, v.y / len};
    }

    inline Vec3 Normalize(Vec3 v){
        float len = Length(v);
        if (len == 0) return Vec3::Zero();
        return {v.x / len, v.y / len, v.z / len};
    }

    inline Matrix TranslateMatrix(Vec3 position){
        Matrix result = Matrix::Identity();

        result.at(3, 0) = position.x; // m12
        result.at(3, 1) = position.y; // m13
        result.at(3, 2) = position.z; // m14

        return result;
    }

    inline Matrix MatrixRotateX(float angle){
        Matrix result = Matrix::Identity();

        float rad = Rads(angle);

        float cos = cosf(rad);
        float sin = sinf(rad);

        result.at(1, 1) = cos; // m5
        result.at(2, 1) = -sin; // m9
        result.at(1, 2) = sin; // m6
        result.at(2, 2) = cos; // m10

        return result;
    }

    inline Matrix MatrixRotateY(float angle){
        Matrix result = Matrix::Identity();

        float rad = Rads(angle);

        float cos = cosf(rad);
        float sin = sinf(rad);

        result.at(0, 0) = cos; // m0
        result.at(2, 0) = sin; // m8
        result.at(0, 2) = -sin; // m2
        result.at(2, 2) = cos; // m10

        return result;
    }

    inline Matrix MatrixRotateZ(float angle){
        Matrix result = Matrix::Identity();

        float rad = Rads(angle);

        float cos = cosf(rad);
        float sin = sinf(rad);

        result.at(0, 0) = cos; // m0
        result.at(1, 0) = -sin; // m4
        result.at(0, 1) = sin; // m1
        result.at(1, 1) = cos; // m5

        return result;
    }

    inline Matrix ScaleMatrix(Vec3 scale){
        Matrix result = Matrix::Identity();

        result.at(0, 0) = scale.x;
        result.at(1, 1) = scale.y;
        result.at(2, 2) = scale.z;

        return result;
    }

    inline Vec3 Cross(Vec3 v1, Vec3 v2){
        return {
           (v1.y*v2.z) - (v1.z*v2.y),
           (v1.z*v2.x) - (v1.x*v2.z),
           (v1.x*v2.y) - (v1.y*v2.x)
        };
    }

    inline float Dot(Vec2 v1, Vec2 v2){
        return (v1.x*v2.x) + (v1.y*v2.y);
    }

    inline float Dot(Vec3 v1, Vec3 v2){
        return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
    }

    // the result is: the rows of the left one, by the cols of the right one
    inline Matrix MultiplyMatrix(Matrix left, Matrix right){
        Matrix result = {};

        // column 0
        result.at(0, 0) =
            left.at(0, 0)*right.at(0, 0) +
            left.at(1, 0)*right.at(0, 1) +
            left.at(2, 0)*right.at(0, 2) +
            left.at(3, 0)*right.at(0, 3);

        result.at(0, 1) =
            left.at(0, 1)*right.at(0, 0) +
            left.at(1, 1)*right.at(0, 1) +
            left.at(2, 1)*right.at(0, 2) +
            left.at(3, 1)*right.at(0, 3);

        result.at(0, 2) =
            left.at(0, 2)*right.at(0, 0) +
            left.at(1, 2)*right.at(0, 1) +
            left.at(2, 2)*right.at(0, 2) +
            left.at(3, 2)*right.at(0, 3);

        result.at(0, 3) =
            left.at(0, 3)*right.at(0, 0) +
            left.at(1, 3)*right.at(0, 1) +
            left.at(2, 3)*right.at(0, 2) +
            left.at(3, 3)*right.at(0, 3); // m3

        // column 1

        result.at(1, 0) =
            left.at(0, 0)*right.at(1, 0) +
            left.at(1, 0)*right.at(1, 1) +
            left.at(2, 0)*right.at(1, 2) +
            left.at(3, 0)*right.at(1, 3);

        result.at(1, 1) =
            left.at(0, 1)*right.at(1, 0) +
            left.at(1, 1)*right.at(1, 1) +
            left.at(2, 1)*right.at(1, 2) +
            left.at(3, 1)*right.at(1, 3);

        result.at(1, 2) =
            left.at(0, 2)*right.at(1, 0) +
            left.at(1, 2)*right.at(1, 1) +
            left.at(2, 2)*right.at(1, 2) +
            left.at(3, 2)*right.at(1, 3);

        result.at(1, 3) =
            left.at(0, 3)*right.at(1, 0) +
            left.at(1, 3)*right.at(1, 1) +
            left.at(2, 3)*right.at(1, 2) +
            left.at(3, 3)*right.at(1, 3); // m7

        // column 2

        result.at(2, 0) =
            left.at(0, 0)*right.at(2, 0) +
            left.at(1, 0)*right.at(2, 1) +
            left.at(2, 0)*right.at(2, 2) +
            left.at(3, 0)*right.at(2, 3);

        result.at(2, 1) =
            left.at(0, 1)*right.at(2, 0) +
            left.at(1, 1)*right.at(2, 1) +
            left.at(2, 1)*right.at(2, 2) +
            left.at(3, 1)*right.at(2, 3);

        result.at(2, 2) =
            left.at(0, 2)*right.at(2, 0) +
            left.at(1, 2)*right.at(2, 1) +
            left.at(2, 2)*right.at(2, 2) +
            left.at(3, 2)*right.at(2, 3);

        result.at(2, 3) =
            left.at(0, 3)*right.at(2, 0) +
            left.at(1, 3)*right.at(2, 1) +
            left.at(2, 3)*right.at(2, 2) +
            left.at(3, 3)*right.at(2, 3); // m11

        // column 3

        result.at(3, 0) =
            left.at(0, 0)*right.at(3, 0) +
            left.at(1, 0)*right.at(3, 1) +
            left.at(2, 0)*right.at(3, 2) +
            left.at(3, 0)*right.at(3, 3);

        result.at(3, 1) =
            left.at(0, 1)*right.at(3, 0) +
            left.at(1, 1)*right.at(3, 1) +
            left.at(2, 1)*right.at(3, 2) +
            left.at(3, 1)*right.at(3, 3);

        result.at(3, 2) =
            left.at(0, 2)*right.at(3, 0) +
            left.at(1, 2)*right.at(3, 1) +
            left.at(2, 2)*right.at(3, 2) +
            left.at(3, 2)*right.at(3, 3);

        result.at(3, 3) =
            left.at(0, 3)*right.at(3, 0) +
            left.at(1, 3)*right.at(3, 1) +
            left.at(2, 3)*right.at(3, 2) +
            left.at(3, 3)*right.at(3, 3); // m15

        return result;
    }

    inline Matrix GetMatrixModel(Vec3& position, Vec3& rotation, Vec3& scale){
        Matrix t = TranslateMatrix(position);
        Matrix rx = MatrixRotateX(rotation.x);
        Matrix ry = MatrixRotateY(rotation.y);
        Matrix rz = MatrixRotateZ(rotation.z);

        Matrix r = MultiplyMatrix(
            MultiplyMatrix(rx, ry), rz
        );

        Matrix s = ScaleMatrix(scale);

        return MultiplyMatrix(
            MultiplyMatrix(t, r), s
        );
    }

    inline Matrix LookAt(Vec3 eye, Vec3 target, Vec3 upChute){
        Vec3 forward = Normalize(Vec3::SubtractVec3(target, eye));
        Vec3 side = Normalize(Cross(forward, upChute));
        Vec3 up = Normalize(Cross(side, forward));

        Matrix result = Matrix::Identity();

        result.m[0][0] = side.x; result.m[1][0] = side.y; result.m[2][0] = side.z;
        result.m[0][1] = up.x; result.m[1][1] = up.y; result.m[2][1] = up.z;
        result.m[0][2] = -forward.x; result.m[1][2] = -forward.y; result.m[2][2] = -forward.z;

        result.m[3][0] = -Dot(side, eye);
        result.m[3][1] = -Dot(up, eye);
        result.m[3][2] = Dot(forward, eye);

        return result;
    }

    inline Matrix OrthoMatrix(float left, float right, float top, float bottom){
        Matrix result = Matrix::Identity();

        result.at(0, 0) = 2.0f / (right - left);
        result.at(1, 1) = 2.0f / (top - bottom);
        result.at(2, 2) = 1.0f;

        result.at(3, 0) = -(right + left) / (right - left);
        result.at(3, 1) = -(top + bottom) / (top - bottom);

        return result;
    }

    inline Matrix GetViewMatrix(Camera2D& cam){
        Matrix t = gmath::TranslateMatrix({-cam.position.x, -cam.position.y, 0.0f});
        Matrix r = gmath::MatrixRotateZ(-cam.rotation);
        Matrix s = gmath::ScaleMatrix({cam.zoom, cam.zoom, 1.0});

        return gmath::MultiplyMatrix(gmath::MultiplyMatrix(t, r), s);
    }

    inline Matrix GetProjectionMatrix(int width, int height){
        return gmath::OrthoMatrix(
            0.0f, static_cast<float>(width),
            0.0f, static_cast<float>(height)
        );
    }

    inline Matrix PerspectiveMatrix(float fov, float aspect, float near, float far){
        float fovrad = Rads(fov);
        float f = 1.0f / tanf(fovrad * 0.5f);

        Matrix result = {};
        result.m[0][0] = f / aspect;
        result.m[1][1] = f;
        result.m[2][2] = -(far + near) / (far - near);
        result.m[2][3] = -1.0f;
        result.m[3][2] = -(2.0f * far * near) / (far - near);

        return result;
    }
};
