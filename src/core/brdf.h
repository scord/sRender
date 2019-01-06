#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/sample.h"

class Brdf {
public:
    Brdf();
    virtual void giveSample(Sample3D sample);
    Sample3D sample;
    virtual Vector3 getValue(Vector3 albedo) = 0;
    virtual Ray getRay(Ray ray, Vector3 p, Vector3 n) = 0;
};