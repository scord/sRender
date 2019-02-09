#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class OrenNayarMaterial : public Material {
public:
    OrenNayarMaterial();
    OrenNayarMaterial(Vector3 albedo);
    OrenNayarMaterial(Vector3 albedo, Vector3 emission);
    OrenNayarMaterial(Vector3 albedo, double roughness);
    Hemisphere hemisphere;
    double roughness;
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Sample3D sample(Vector3 dir, Vector3 odir, Vector3 n, Sampler* sampler);
};

