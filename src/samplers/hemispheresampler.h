#pragma once

#include "../core/sampler.h"
#include <vector>
#include "../core/smath.h"

class HemisphereSampler {
public:
    HemisphereSampler(int nSamples);
    HemisphereSampler();
    Sampler sampler;
    Sample3D getSample();
};