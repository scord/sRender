#include "diffuse.h"

Vector3 DiffuseBrdf::getValue(Vector3 albedo) {
    return albedo / PI;
}

void DiffuseBrdf::giveSample(Sample3D sample) {
    this->sample = sample;
}

Ray DiffuseBrdf::getRay(Ray ray, Vector3 p, Vector3 n) {
    Ray newRay(p, sample.value);
    newRay.rotateToVector(n);
    return newRay;
}

