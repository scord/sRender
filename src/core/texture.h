#pragma once

#include "image.h"
#include "smath.h"
#include <functional>

class Texture {
public:
    
    Image image;
    std::function<Vector2(Vector3)> uvMapping;
    Texture() {};
    Texture(Image& image) : image(image) {};

    Vector3 sample(Vector3 p) {
        return image.sample(uvMapping(p));
    };
};