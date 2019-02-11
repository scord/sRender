#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class GGXMaterial : public Material {
public:
    GGXMaterial();
    GGXMaterial(Vector3 albedo);
    GGXMaterial(Vector3 albedo, Vector3 emission);
    GGXMaterial(Vector3 albedo, double roughness);
    Hemisphere hemisphere;
    double roughness;
    double alpha;
    double alpha2;
    double distribution(double cost);
    double masking(double cost);
    double fresnel(double etai, double etat, double cost);
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Sample3D sample(Vector3 dir, Vector3 odir, Vector3 n, Sampler* sampler);
};

