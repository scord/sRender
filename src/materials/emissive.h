#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class EmissiveMaterial : public Material {
public:
    EmissiveMaterial();
    EmissiveMaterial(vec3 emission);
    EmissiveMaterial(Texture texture, vec3 emission);
    virtual double getPdf(vec3 dir, vec3 odir, vec3 n);
    virtual vec3 getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv);
    virtual SampleBSDF sample(vec3 dir, vec3 n, Sampler& sampler);
    Texture texture;


};


