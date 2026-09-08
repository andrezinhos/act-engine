#pragma once

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

class ios{
private:
    static int curr[348 + 1];
    static int prev[348 + 1];
public:
	static void InputUpdate();
    static bool KeyDown(Keys key);
    static bool KeyPressed(Keys key);
};
