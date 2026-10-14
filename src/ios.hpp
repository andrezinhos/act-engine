#pragma once
#include <cstdint>

class string_view;

enum class Keys {
    W = 87,
    A = 65,
    S = 83,
    D = 68,

    Q = 81,
    E = 69,

    LEFT = 263,
    RIGHT = 262,
    UP = 265,
    DOWN = 264,

    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
};

struct Image{
    uint8_t* data;
    int width, height, channels;
};

class ios{
public:
    static uint8_t* LoadBytes(std::string_view path, size_t& size);

	static void InputUpdate();
    static bool KeyDown(Keys key);
    static bool KeyPressed(Keys key);

    static Image LoadImage(std::string_view path);
    static void DeleteImage(Image& image);
};
