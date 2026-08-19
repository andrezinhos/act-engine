#pragma once
#include <cmath>

constexpr float PI = 3.14159265358979323846f;

struct Vec2{
    float x, y;

    Vec2(float x, float y) : x(x), y(y) {}

    static Vec2 Zero(){
        return {0.0f, 0.0f};
    }

    static Vec2 One(){
        return {1.0f, 1.0f};
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
};

struct Vec4{
    float x, y, z, w;
};

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
    Vec3 position = Vec3::Zero();
    Vec3 rotation = Vec3::Zero();
    Vec3 scale = Vec3::One();
};

namespace gmath{

    inline float rads(float angle){
        float result = angle * PI / 180.0f;

        return result;
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

    inline float CastToFloat(int value){
        return static_cast<float>(value);
    }

    inline Vec2 CastToVec2(int v1, int v2){
        float x = CastToFloat(v1);
        float y = CastToFloat(v2);
        Vec2 result = {x, y};
        return result;
    }

    inline Vec2 AddVec2(Vec2 v1, Vec2 v2){
        Vec2 result = {v1.x + v2.x, v1.y + v2.y};
        return result;
    }

    inline Vec2 SubtractVec2(Vec2 v1, Vec2 v2){
        Vec2 result = {v1.x - v2.x, v1.y - v2.y};
        return result;
    }

    inline float LengthVec2(Vec2 v){
        float result = sqrtf(v.x*v.x + v.y*v.y);
        return result;
    }

    inline void IncreaseVec2(Vec2& v1, Vec2& v2){
        v1.x += v2.x;
        v1.y += v2.y;
    }

    inline void DecreaseVec2(Vec2& v1, Vec2& v2){
        v1.x -= v2.x;
        v1.y -= v2.y;
    }

    inline Vec3 AddVec3(Vec3 v1, Vec3 v2){
        Vec3 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
        return result;
    }

    inline Vec3 SubtractVec3(Vec3 v1, Vec3 v2){
        Vec3 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
        return result;
    }

    inline float LengthVec3(Vec3 v){
        float result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        return result;
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

        float rad = rads(angle);

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

        float rad = rads(angle);

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

        float rad = rads(angle);

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

    inline Matrix GetMatrix(Vec3& position, Vec3& rotation, Vec3& scale){
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

    inline Matrix OrthoMatrix(float left, float right, float top, float bottom){
        Matrix result = Matrix::Identity();

        result.at(0, 0) = 2.0f / (right - left);
        result.at(1, 1) = 2.0f / (top - bottom);
        result.at(2, 2) = 1.0f;

        result.at(3, 0) = -(right + left) / (right - left);
        result.at(3, 1) = -(top + bottom) / (top - bottom);

        return result;
    }
};
