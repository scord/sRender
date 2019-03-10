#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial();
    DiffuseMaterial(Vector3 albedo);
    DiffuseMaterial(Vector3 albedo, Vector3 emission);
    Hemisphere hemisphere;
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual SampleBSDF sample(Vector3 dir, Vector3 n, Sampler* sampler);
};

