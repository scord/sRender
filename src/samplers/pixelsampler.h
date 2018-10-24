#pragma once

#include "../core/sampler.h"
#include <vector>
#include "../core/smath.h"

class PixelSampler : public Sampler {
public:
    PixelSampler(int samplesPerPixel);
    double getdoubleSample();
    Vector2 getVectorSample();
    Vector2 getPixelSample(const Vector2 &p);
    int sampleNumber;
    void generateTestImage(const int width, const int height);
protected:
    std::vector<double> doubleSamples;
    std::vector<Vector2> vectorSamples;
};