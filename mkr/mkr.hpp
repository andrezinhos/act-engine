#pragma once
#include "fgl.h"
#include "mkgl.hpp"
#include "glfw/glfw3.h"
#include "mktxt.hpp"
#include <vector>

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

struct Mesh{
    fgl_buffer buffer;
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
};

typedef struct {
    Cursor cursor;
    GLFWimage icon;
	GLFWmonitor* moni;
	const GLFWvidmode* mode;
    GLFWwindow* main;
    int win_width, win_height;
} Window;

struct Batch{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<DCall> calls;
    uint32_t vao, vbo, ebo;
};

struct DState{
    Batch dbatch;
    Texture dtex;
    Mesh dmesh;
    Shader dshader;
    Font dfont;
};

extern int flags_active[3];
extern Window wmain;
extern DState* dstate;

class mkr{
private:
    static bool createWindowContext();
    static void setWindowPosition(int width, int height);
    static void sendVertex(Vec2 position, Vec2 size, color color, Vec2 uv);
    static void sendVertex(Vec2 position, Vec2 size, color color, float u0, float u1, float v0, float v1);
    static void sendIndices(uint32_t base);
    static void drawElements(size_t count, void* offset);
    static void limitFlush();
    static void flush();
public:
    static bool configInit();

    static void setWindowIcon(const char* path);
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

    static Shader LoadShader(const char* vs, const char* fs);
    static Font LoadFontSrc(const char* path);
    static void UnloadShader(const Shader& shader);
    static void ScreenClear(color color);

    static void RenderRectangle(Vec2 position, Vec2 size, color color);
    static void RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, color color);
    static void RenderTexture(Texture* tex, Vec2 position, Vec2 size, color color);

    static void RenderBegin();
    static void RenderEnd();
    static void CameraBegin(Camera2D& cam);
    static void CameraEnd();
};
