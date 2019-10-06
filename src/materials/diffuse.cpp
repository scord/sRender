#include "diffuse.h"

DiffuseMaterial::DiffuseMaterial() : Material(), hemisphere(vec3(), 1) {
}
DiffuseMaterial::DiffuseMaterial(vec3 albedo) : Material(albedo, vec3()), hemisphere(vec3(), 1) {
}
DiffuseMaterial::DiffuseMaterial(vec3 albedo, vec3 emission) : Material(albedo, emission), hemisphere(vec3(), 1) {
}


vec3 DiffuseMaterial::getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv) {
    return albedo.value(uv)*std::abs(dir.dot(n))*IPI;
}

SampleBSDF DiffuseMaterial::sample(vec3 odir, vec3 n, Sampler& sampler) {
    Sample3D sample = hemisphere.map(sampler.getSample2D());
    vec3 idir = sample.value.rotate(n);
    return SampleBSDF(sample.value.rotate(n), vec3(), sample.value.z, sample.pdf);
}

double DiffuseMaterial::getPdf(vec3 dir, vec3 odir, vec3 n) {
    return std::abs(dir.dot(n))*IPI;
}

