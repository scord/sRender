#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"


class DiffuseBrdf : public Brdf {
public:
    DiffuseBrdf() {};
    HemisphereSampler sampler;
    Sample3D sample;
    virtual double getPdf(Ray ray, Vector3 p, Vector3 n);
    virtual Vector3 getValue(Vector3 albedo);
    virtual Sample3D getSample(double u1, double u2);
    virtual Ray getRay(Ray ray, Vector3 p, Vector3 n);
};
