#pragma once

#include "smath.h"

class Sampler {
public:
    int samplesPerPixel;
    Sampler(int samplesPerPixel);
    virtual double getdoubleSample() = 0;
    virtual Vector2 getVectorSample() = 0;

    Vector2 getPixelSample(const Vector2 &p);


};