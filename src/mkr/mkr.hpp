#pragma once
#include "mkgl.hpp"
#include "glfw/glfw3.h"

struct Window{
	GLFWmonitor* moni = nullptr;
	const GLFWvidmode* mode = nullptr;
    GLFWwindow* main = nullptr;
    int win_width, win_height;
};

class mkr{
public:
    static int flags_active[4];
    static Window wmain;
    static bool startWindow(int width, int height, const char* title);

    static void Initialize();
    static void Shutdown();
    static Shader DefaultShader();
    static Mesh DefaultQuad();
    static Texture DefaultTexture();
    static void DefaultBatch();

    static void UnloadDefaultShader();
    static void UnloadDefaultQuad();
    static void UnloadDefaultTexture();
    static void UnloadDefaultBatch();

    static Texture LoadTexture(const char* path);
    static void UnloadTexture(Texture& tex);
    static void RenderTexture(Texture *tex, Vec2 position, Vec2 size, Color color);
    static void RenderBegin();
    static void RenderEnd();
    static void CameraBegin(Camera2D& cam);
    static void CameraEnd();
};
