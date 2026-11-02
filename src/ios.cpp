#include "core.hpp"
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

bool engine::image::load(std::string_view path){
    size_t size = 0;
    uint8_t* file = io::load_bytes(path, size);

    if (!file) {
        std::cout << "error: image do not exist\n";
        return false;
    }

    _data = stbi_load_from_memory(
        file,
        static_cast<int>(size),
        &_width,
        &_height,
        &_ch,
        4
    );

    if (!_data){
        std::cout << "error: failed to load image: " << stbi_failure_reason() << "\n";
        delete[] file;
        file = nullptr;
        return false;
    }

    delete[] file;
    file = nullptr;

    return true;
}

engine::image::~image(){
    stbi_image_free(_data);
    _data = nullptr;
    _width = 0;
    _height = 0;
}

int engine::image::width() const {
    return _width;
}

int engine::image::height() const {
    return _height;
}

int engine::image::channels() const {
    return _ch;
}

uint8_t* engine::image::data() const {
    return _data;
}
