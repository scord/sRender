#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"

class SpecularBrdf : public Brdf {
public:
    SpecularBrdf() {};
    Sample3D sample;
    double eta = 1;
    double fresnel(double cosi, double etai, double etat);
    virtual double getPdf(Ray ray, Vector3 p, Vector3 n);
    virtual Vector3 getValue(Vector3 albedo);
    virtual Sample3D getSample(double u1, double u2);
    virtual Ray getRay(Ray ray, Vector3 p, Vector3 n);
};