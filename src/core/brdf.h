#pragma once

#include "../core/smath.h"
#include "../core/geometry.h"
#include <random>
#include "../core/sample.h"

class Brdf {
public:
    Brdf();
    virtual vec3 getValue(vec3 albedo) = 0;
    virtual Sample3D getSample(double u1, double u2) = 0;
    virtual Ray getRay(Ray ray, vec3 p, vec3 n) = 0;
    virtual double getPdf(Ray ray, vec3 p, vec3 n) = 0;
};