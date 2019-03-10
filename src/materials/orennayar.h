#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"
#include "../core/texture.h"

class OrenNayarMaterial : public Material {
public:
    OrenNayarMaterial();
    OrenNayarMaterial(Vector3 albedo);
    OrenNayarMaterial(Vector3 albedo, Vector3 emission);
    OrenNayarMaterial(Vector3 albedo, double roughness);
    OrenNayarMaterial(Image& texture);

    Hemisphere hemisphere;
    double roughness;
    Image texture;
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector3 p);
    virtual SampleBSDF sample(Vector3 dir, Vector3 n, Sampler* sampler);
    virtual Vector3 getAlbedo(Vector3 p);
};

