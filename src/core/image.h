#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "smath.h"

class Image {
public:
    Image(std::vector<std::vector<Vector3>> pixels) : pixels(pixels) {
        width = pixels[0].size();
        height = pixels.size();
    };
    Image() {

    };
    ~Image() = default;

    Vector3 sample(Vector2 uv) {
        int x = (int)(uv.x*width);
        if (x == width)
            x = width - 1;

        int y = (int)(uv.y*height);
        if (y == height)
            y = height - 1;

        return pixels[y][x];
    }

    int width;
    int height;
    std::vector<std::vector<Vector3>> pixels;

};