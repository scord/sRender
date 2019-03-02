#include "sample.h"

Sample2D::Sample2D(Vector2 v, double pdf) : value(v), pdf(pdf) {}
Sample2D::Sample2D() {}
Sample3D::Sample3D(Vector3 v, double pdf) : value(v), pdf(pdf) {}
Sample3D::Sample3D() {}
SampleBSDF::SampleBSDF(Vector3 v, Vector3 bsdf, double cost, double pdf) : value(v), bsdf(bsdf), pdf(pdf), cost(cost) {}
SampleBSDF::SampleBSDF() {}