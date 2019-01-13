#pragma once

#include "../core/sampler.h"
#include <vector>
#include "../core/smath.h"

class DiscSampler : public Sampler {
public:
    DiscSampler(int nSamples) : Sampler::Sampler(nSamples) {}
    Sampler Sampler;
    Sample2D getSample();
    int nSamples;
    int sampleNumber;
    void generateTestImage(const int width, const int height);
protected:
    std::vector<Sample2D> vectorSamples;
};