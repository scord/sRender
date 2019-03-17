#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "smath.h"

class Texture {
public:
    Texture(std::vector<std::vector<Vector3>> pixels) : pixels(pixels) {
        width = pixels[0].size();
        height = pixels.size();
    };
    Texture() {

    };
    ~Texture() = default;

    Vector3 sample(Vector2 uv) {
        int x = (int)(uv.x*width);
        if (x == width)
            x = width - 1;

        if (x < 0) {
            x = 0;
        }

        int y = (int)(uv.y*height);
        if (y == height)
            y = height - 1;

        if (y < 0) {
            y = 0;
        }

        return pixels[y][x];
    }

    int width;
    int height;
    std::vector<std::vector<Vector3>> pixels;

};