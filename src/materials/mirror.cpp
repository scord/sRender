#include "mirror.h"

MirrorMaterial::MirrorMaterial() : Material() {
}
MirrorMaterial::MirrorMaterial(vec3 albedo) : Material(albedo, vec3()) {
}

vec3 MirrorMaterial::getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv) {
    return albedo.value(uv);
}

SampleBSDF MirrorMaterial::sample(vec3 dir,  vec3 n, Sampler& sampler) {
    dir = vec3() - dir;
    vec3 reflectedDir = (dir - n*dir.dot(n)*2).norm();
    return SampleBSDF(reflectedDir, vec3(), 1, 1);
}

double MirrorMaterial::getPdf(vec3 dir, vec3 odir, vec3 n) {
    return 0;
}

