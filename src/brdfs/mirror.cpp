#include "mirror.h"

Ray MirrorBrdf::getRay(Ray ray, Vector3 p, Vector3 n) {
    Vector3 reflectedDir = (ray.direction - n*ray.direction.dot(n)*2).norm();
    return Ray(p, reflectedDir);
}

Sample3D MirrorBrdf::getSample(double u1, double u2) {
    sample = Sample3D();
    sample.pdf = 1;
    sample.value.z = 1;
    return sample;
}

Vector3 MirrorBrdf::getValue(Vector3 albedo) {
    return Vector3(1,1,1);
}
