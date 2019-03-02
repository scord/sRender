#pragma once
#include "smath.h"

class Sample {
};

class Sample2D : public Sample {
public:
    Vector2 value;
    Sample2D();
    Sample2D(Vector2 v, double pdf);
    double pdf;
};

class Sample3D : public Sample {
public:
    Vector3 value;
    Sample3D();
    Sample3D(Vector3 v, double pdf);
    double pdf;
};

class SampleBSDF : public Sample {
public:
    Vector3 value;
    SampleBSDF();
    SampleBSDF(Vector3 v, Vector3 bsdf, double cost, double pdf);
    ~SampleBSDF() = default;
    double pdf;
    double cost;
    Vector3 bsdf;
};