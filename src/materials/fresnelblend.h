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
    FresnelBlend(vec3 albedo, vec3 specularAlbedo, double roughness);
    FresnelBlend(Texture& texture, vec3 specularAlbedo, double roughness);

    MaterialProperty<double> roughness;
    MaterialProperty<vec3> specularAlbedo;
    MaterialProperty<vec3> diffuseAlbedo;

    double alpha;
    double alpha2;
    Hemisphere hemisphere;

    double distribution(double cost);
    double g1(double cost);
    double g1DividedBy2Cos(double cost) ;
    double g2(double costi, double costr);
    double g2DividedBy2CosiCoso(double costi, double costo);
    double fresnel(double etai, double etat, double cost);
    vec3 fresnel(vec3 r0, double cost);
    virtual double getPdf(vec3 dir, vec3 odir, vec3 n);
    virtual vec3 getBrdf(vec3 dir, vec3 odir, vec3 n);
    virtual vec3 getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv);

    virtual SampleBSDF sample(vec3 dir, vec3 n, Sampler& sampler);
    SampleBSDF sampleDiffuse(vec3 dir, vec3 n, Sampler& sampler);
    SampleBSDF sampleSpecular(vec3 dir, vec3 n, Sampler& sampler);


    vec3 getBrdf(double nidir, double nodir, double mdir, double d);
  

};

