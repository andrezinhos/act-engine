#pragma once
#include "mkgl.hpp"
#include "glfw/glfw3.h"
#include "mkr.hpp"

enum Cursor{
    NORMAL,
    HIDDEN,
    DISABLED
};

struct Rectangle{
    float x, y;
    float width, height;
};

struct Window{
    Cursor cursor;
    GLFWimage icon;
	GLFWmonitor* moni = nullptr;
	const GLFWvidmode* mode = nullptr;
    GLFWwindow* main = nullptr;
    int win_width, win_height;
};

class mkr{
private:
    static bool createWindowContext();
    static void setWindowPosition(int width, int height);
    static void GenTexture(Texture& tex, const void* data, int width, int height, GLenum format);
public:
    static int flags_active[4];
    static Window wmain;
    static void setWindowIcon(const char* path);
    static void setCursorMode(Cursor cur);
    static bool startWindow(int width, int height, const char* title);

    static int GetWindowWidth();
    static int GetWindowHeight();

    static void Initialize();
    static void Shutdown();

    static Shader DefaultShader();
    static Mesh DefaultQuad();
    static Texture DefaultTexture();
    static Font DefaultFont();
    static void DefaultBatch();

    static void UnloadDefaultShader();
    static void UnloadDefaultQuad();
    static void UnloadDefaultTexture();
    static void UnloadDefaultFont();
    static void UnloadDefaultBatch();

    static Shader LoadShader(const char* vs, const char* fs);
    static Texture LoadTextureSrc(const char* path);
    static Font LoadFontSrc(const char* path);
    
    static void UnloadTexture(const Texture& tex);
    static void UnloadShader(const Shader& shader);
    static void UnloadFont(const Font& font);
    static void ScreenClear(Color color);

    static void RenderRectangle(Vec2 position, Vec2 size, Color color);
    static void RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, Color color);
    static void RenderTexture(Texture* tex, Vec2 position, Vec2 size, Color color);

    static void RenderText(const std::string& text, Vec2 position, Vec2 size, Color color);
    static void RenderBegin();
    static void RenderEnd();
    static void CameraBegin(Camera2D& cam);
    static void CameraEnd();
};
