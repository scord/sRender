#include "mirror.h"

Ray MirrorBrdf::getRay(Ray ray, Vector3 p, Vector3 n) {
    Vector3 reflectedDir = (ray.direction - n*ray.direction.dot(n)*2).norm();
    return Ray(p, reflectedDir);
}

Vector3 MirrorBrdf::getValue(Vector3 albedo) {
    return albedo;
}
