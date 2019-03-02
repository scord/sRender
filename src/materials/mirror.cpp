#include "mirror.h"

MirrorMaterial::MirrorMaterial() {
}
MirrorMaterial::MirrorMaterial(Vector3 albedo) {
    this->albedo = albedo;
}

Vector3 MirrorMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return albedo;
}

SampleBSDF MirrorMaterial::sample(Vector3 dir,  Vector3 n, Sampler* sampler) {
    dir = Vector3() - dir;
    Vector3 reflectedDir = (dir - n*dir.dot(n)*2).norm();
    return SampleBSDF(reflectedDir, albedo, 1, 1);
}

double MirrorMaterial::getPdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return 0;
}

