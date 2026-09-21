#pragma once
#include "mkgl.hpp"
#include "glfw/glfw3.h"
#include "mktxt.hpp"

enum Cursor{
    NORMAL,
    HIDDEN,
    DISABLED
};

struct Rectangle{
    float x, y;
    float width, height;
};

struct Mesh{
    uint vao, vbo, ebo;
    std::vector<vertex> vertices;
    std::vector<uint> indices;
};

struct Window{
    Cursor cursor;
    GLFWimage icon;
	GLFWmonitor* moni = nullptr;
	const GLFWvidmode* mode = nullptr;
    GLFWwindow* main = nullptr;
    int win_width, win_height;
};

struct DState{
    Batch dbatch;
    Texture dtex;
    Mesh dmesh;
    Shader dshader;
    Font dfont;
};

class mkr{
private:
    static bool createWindowContext();
    static void setWindowPosition(int width, int height);
    static void sendVertex(Vec2 position, Vec2 size, Color color, Vec2 uv);
    static void sendVertex(Vec2 position, Vec2 size, Color color, float u0, float u1, float v0, float v1);
    static void sendIndices(uint base);
    static void drawElements(size_t count, void* offset);
    static void limitFlush();
    static void flush();
public:
    static int flags_active[3];
    static Window wmain;
    static DState state;
    static void setWindowIcon(cstr path);
    static void setCursorMode(Cursor cur);
    static bool startWindow(int width, int height, const char* title);

    static int GetWindowWidth();
    static int GetWindowHeight();

    static void Initialize();
    static void Shutdown();

    static Shader DefaultShader();
    static Mesh DefaultQuad();
    static void DefaultBatch();

    static void UnloadDefaultShader();
    static void UnloadDefaultQuad();
    static void UnloadDefaultBatch();

    static Shader LoadShader(cstr vs, cstr fs);
    static Font LoadFontSrc(cstr path);
    static void UnloadShader(const Shader& shader);
    static void ScreenClear(Color color);

    static void RenderRectangle(Vec2 position, Vec2 size, Color color);
    static void RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, Color color);
    static void RenderTexture(Texture* tex, Vec2 position, Vec2 size, Color color);

    static void RenderBegin();
    static void RenderEnd();
    static void CameraBegin(Camera2D& cam);
    static void CameraEnd();
};
