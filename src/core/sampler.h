#pragma once

#include <vector>
#include "../core/smath.h"
#include "sample.h"
#include <random>

class Sampler {
public:
    Sampler() {};
    Sampler(int nSamples);
    Sample2D getSample();
    int sampleNumber;
    int nSamples;
    void generateTestImage(const int width, const int height);
protected:
    std::vector<Sample2D> vectorSamples;
};