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
    OrenNayarMaterial(vec3 albedo);
    OrenNayarMaterial(vec3 albedo, vec3 emission);
    OrenNayarMaterial(vec3 albedo, double roughness);
    OrenNayarMaterial(Texture& texture);

    Hemisphere hemisphere;
    double roughness;
    Texture texture;
    virtual double getPdf(vec3 dir, vec3 odir, vec3 n);
    virtual vec3 getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv);
    virtual SampleBSDF sample(vec3 dir, vec3 n, Sampler& sampler);
};

