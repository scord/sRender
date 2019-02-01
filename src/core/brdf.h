#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/sample.h"

class Brdf {
public:
    Brdf();
    virtual Vector3 getValue(Vector3 albedo) = 0;
    virtual Sample3D getSample(double u1, double u2) = 0;
    virtual Ray getRay(Ray ray, Vector3 p, Vector3 n) = 0;
    virtual double getPdf(Ray ray, Vector3 p, Vector3 n) = 0;
};