#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class MirrorMaterial : public Material {
public:
    MirrorMaterial();
    MirrorMaterial(Vector3 albedo);
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector2 uv);
    virtual SampleBSDF sample(Vector3 dir, Vector3 n, Sampler* sampler);
};

