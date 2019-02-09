#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class SpecularMaterial : public Material {
public:
    SpecularMaterial();
    SpecularMaterial(Vector3 albedo);

    double fresnel(double cosi, double etai, double etat) ;
    
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Sample3D sample(Vector3 dir, Vector3 odir, Vector3 n, Sampler* sampler);
};

