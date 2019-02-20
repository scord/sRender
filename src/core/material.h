#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/sample.h"
#include "../samplers/sampler.h"

class Material {
public:
    Material();
    Vector3 albedo;
    Vector3 emission;
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n) = 0;
    virtual Sample3D sample(Vector3 dir, Vector3 odir, Vector3 n, Sampler* sampler) = 0;
    virtual Sample3D sample(Vector3 dir, Vector3 odir, Vector3 n, Vector3& reflectance, Sampler* sampler);
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n) = 0;
    virtual Vector3 reflectance(Vector3 dir, Vector3 odir, Vector3 n);
};