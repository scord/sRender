#include "diffuse.h"

Vector3 DiffuseBrdf::getValue(Vector3 albedo) {
    return albedo / (PI);
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

double DiffuseBrdf::getPdf(Ray ray, Vector3 p, Vector3 n) {
    return std::abs(ray.direction.dot(n)/(ray.direction.length()*n.length()))*IPI;
}

