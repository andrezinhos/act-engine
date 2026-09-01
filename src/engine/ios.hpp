#pragma once

enum class Keys {
    W = 87,
    A = 65,
    S = 83,
    D = 68,

    Q = 81,
    E = 69,
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
