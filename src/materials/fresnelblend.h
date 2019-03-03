#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"
#include "../samplers/sampler.h"
#include "../core/geometry.h"
#include "../core/material.h"

class FresnelBlend : public Material {
public:
    FresnelBlend();
    FresnelBlend(Vector3 albedo);
    FresnelBlend(Vector3 albedo, Vector3 emission);
    FresnelBlend(Vector3 albedo, double roughness);
    FresnelBlend(Vector3 albedo, Vector3 specularAlbedo, double roughness);
    double roughness;
    double alpha;
    double alpha2;
    Hemisphere hemisphere;
    Vector3 specularAlbedo;
    double distribution(double cost);
    double g1(double cost);
    double g1DividedBy2Cos(double cost) ;
    double g2(double costi, double costr);
    double g2DividedBy2CosiCoso(double costi, double costo);
    double fresnel(double etai, double etat, double cost);
    Vector3 fresnel(Vector3 r0, double cost);
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n);
    virtual SampleBSDF sample(Vector3 dir, Vector3 n, Sampler* sampler);
    SampleBSDF sampleDiffuse(Vector3 dir, Vector3 n, Sampler* sampler);
    SampleBSDF sampleSpecular(Vector3 dir, Vector3 n, Sampler* sampler);

    Vector3 getBrdf(double nidir, double nodir, double mdir, double d);

};

