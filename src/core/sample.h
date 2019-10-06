#pragma once
#include "smath.h"



template <typename T>
class Sample{
public:
    T value;
    Sample<T>();
    Sample<T>(T t, double pdf) : value(t), pdf(pdf) {};
    double pdf;
};




class Sample2D {
public:
    vec2 value;
    Sample2D();
    Sample2D(vec2 v, double pdf);
    double pdf;
};

class Sample3D {
public:
    vec3 value;
    Sample3D();
    Sample3D(vec3 v, double pdf);
    double pdf;
};

class SampleBSDF {
public:
    vec3 value;
    SampleBSDF();
    SampleBSDF(vec3 v, vec3 bsdf, double cost, double pdf);
    ~SampleBSDF() = default;
    double pdf;
    double cost;
    vec3 bsdf;
};