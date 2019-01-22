#pragma once

#include "../core/sampler.h"
#include <vector>
#include "../core/smath.h"

class HemisphereSampler {
public:
    HemisphereSampler();
    Sample3D getSample(double u1, double u2);
};