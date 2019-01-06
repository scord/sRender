#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"

class MirrorBrdf : public Brdf {
public:
    MirrorBrdf() {};
    Sample3D sample;
    virtual Vector3 getValue(Vector3 albedo);
    virtual Ray getRay(Ray ray, Vector3 p, Vector3 n);
};