#include "sample.h"


Sample2D::Sample2D(vec2 v, double pdf) : value(v), pdf(pdf) {}
Sample2D::Sample2D() {}
Sample3D::Sample3D(vec3 v, double pdf) : value(v), pdf(pdf) {}
Sample3D::Sample3D() {}
SampleBSDF::SampleBSDF(vec3 v, vec3 bsdf, double cost, double pdf) : value(v), bsdf(bsdf), pdf(pdf), cost(cost) {}
SampleBSDF::SampleBSDF() {}