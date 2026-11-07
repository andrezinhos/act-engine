#include "core.hpp"
#include "fgl.h"
#include "glfw/glfw3.h"
#include "mkr.hpp"
#include <array>
#include <fstream>
#include <iostream>
#define STBI_NO_GIF
#define STBI_NO_PNM
#define STBI_NO_BMP
#define STBI_NO_PIC
#define STBI_NO_PSD
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr int MAXKEY = 350;

std::array<int, MAXKEY> prev = {};
std::array<int, MAXKEY> curr = {};

void frame_callback(GLFWwindow* win, int width, int height){
    if (width == 0 || height == 0) {
        return;
    }

    glViewport(0, 0, width, height);

    engine::core::mainw.width = width;
    engine::core::mainw.height = height;
}

uint8_t* engine::io::load_bytes(std::string_view path, size_t& size){
    std::ifstream file;
    file.open(path.data(), std::ios::binary);
    if (!file.is_open()) return nullptr;

    file.seekg(0, std::ios::end);
    size_t fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    uint8_t* buffer = new uint8_t[fsize];

    file.read(reinterpret_cast<char*>(buffer), fsize);
    file.close();

    size = fsize;

    return buffer;
}

void engine::io::input_update(){
    prev = curr;

	for (int key = 0; key < MAXKEY; key++){
		curr.at(key) = glfwGetKey(wmain.main, key) == 1;
	}
}

bool engine::io::key_down(engine::keys key){
	return curr[static_cast<int>(key)];
}

bool engine::io::key_pressed(engine::keys key){
	return curr[static_cast<int>(key)] && !prev[static_cast<int>(key)];
}

bool image::load(std::string_view path){
    size_t size = 0;
    uint8_t* file = engine::io::load_bytes(path, size);

    if (!file) {
        std::cout << "error: image do not exist\n";
        return false;
    }

    data = stbi_load_from_memory(
        file,
        size,
        &width,
        &height,
        &ch,
        4
    );

    if (!data){
        std::cout << "error: failed to load image: " << stbi_failure_reason() << "\n";
        delete[] file;
        file = nullptr;
        return false;
    }

    delete[] file;
    file = nullptr;

    return true;
}

image::~image(){
    stbi_image_free(data);
    data = nullptr;
    width = 0;
    height = 0;
}

bool window::init(int w, int h, std::string_view t){
    if (!glfwInit()){
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    data = glfwCreateWindow(w, h, t.data(), moni, nullptr);
    if (!data){
        std::cout << "error: failed to create window\n";
        glfwTerminate();
        return false;
    }

    width = w;
    height = h;

    return true;
}

bool window::should_close(){
    bool result;

    if (glfwWindowShouldClose(data)){
        result = true;
    }
    else {
        result = false;
    }

    return result;
}

void window::destroy(){
    glfwDestroyWindow(data);
}

bool window::context(){
    glfwSetFramebufferSizeCallback(data, frame_callback);
    glfwMakeContextCurrent(data);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "error: failed to create window context\n";
        glfwDestroyWindow(data);
        glfwTerminate();
        return false;
    }

    int fbw = 0;
    int fbh = 0;
    glfwGetFramebufferSize(data, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    width = fbw;
    height = fbh;

    return true;
}

void window::swap(){
    glfwSwapBuffers(data);
}

void window::pos_center(){
    if (flags_active[2] == 1){
        moni = glfwGetPrimaryMonitor();
        vid = glfwGetVideoMode(moni);

        int centerx = (vid->width/2) - (width/2);
        int centery = (vid->height/2) - (height/2);

        glfwSetWindowPos(data, centerx, centery);
    }
}
