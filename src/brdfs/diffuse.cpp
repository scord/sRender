#include "diffuse.h"

Vector3 DiffuseBrdf::getValue(Vector3 albedo) {
    return albedo * 0.4 / PI;
}

Sample3D DiffuseBrdf::getSample(double u1, double u2) {
    sample = sampler.getSample(u1, u2);
    return sample;
}

Ray DiffuseBrdf::getRay(Ray ray, Vector3 p, Vector3 n) {
    Ray newRay(p, sample.value);
    newRay.rotateToVector(n);
    return newRay;
}

