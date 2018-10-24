#include "mirror.h"

MirrorBrdf::MirrorBrdf(){
    pdf = 1.0;
}

Ray MirrorBrdf::Sample(Ray ray, Vector3 p, Vector3 n) {
	Vector3 reflectedDir = (ray.direction - n*ray.direction.dot(n)*2).norm();
    return Ray(p, reflectedDir);
}