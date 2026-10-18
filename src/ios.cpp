#include "ios.hpp"
#include "mkr.hpp"
#include <array>
#include <string_view>
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

uint8_t* ios::LoadBytes(std::string_view path, size_t& size){
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

void ios::InputUpdate(){
    prev = curr;

	for (int key = 0; key < MAXKEY; key++)
		curr.at(key) = glfwGetKey(wmain.main, key) == 1;
}

bool ios::KeyDown(Keys key){
	return curr[static_cast<int>(key)];
}

bool ios::KeyPressed(Keys key){
	return curr[static_cast<int>(key)] && !prev[static_cast<int>(key)];
}

Image ios::LoadImage(std::string_view path){
    Image image = {};

    size_t size = 0;
    uint8_t* file = LoadBytes(path, size);

    if (!file) {
        std::cout << "Image Not Founded\n";
        return image;
    }

    image.data = stbi_load_from_memory(
        file,
        static_cast<int>(size),
        &image.width,
        &image.height,
        &image.channels,
        4
    );

    if (!image.data){
        std::cout << "Image Not Loaded: " << stbi_failure_reason() << "\n";
        delete[] file;
        file = nullptr;
        return image;
    }

    delete[] file;
    file = nullptr;

    return image;
}

void ios::DeleteImage(Image &image){
    if (image.data) stbi_image_free(image.data);
}
