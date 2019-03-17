#include "mirror.h"

MirrorMaterial::MirrorMaterial() : Material() {
}
MirrorMaterial::MirrorMaterial(Vector3 albedo) : Material(albedo, Vector3()) {
}

Vector3 MirrorMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector2 uv) {
    return albedo.value(uv);
}

SampleBSDF MirrorMaterial::sample(Vector3 dir,  Vector3 n, Sampler* sampler) {
    dir = Vector3() - dir;
    Vector3 reflectedDir = (dir - n*dir.dot(n)*2).norm();
    return SampleBSDF(reflectedDir, Vector3(), 1, 1);
}

double MirrorMaterial::getPdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return 0;
}

