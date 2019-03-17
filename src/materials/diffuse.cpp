#include "diffuse.h"

DiffuseMaterial::DiffuseMaterial() : Material(), hemisphere(Vector3(), 1) {
}
DiffuseMaterial::DiffuseMaterial(Vector3 albedo) : Material(albedo, Vector3()), hemisphere(Vector3(), 1) {
}
DiffuseMaterial::DiffuseMaterial(Vector3 albedo, Vector3 emission) : Material(albedo, emission), hemisphere(Vector3(), 1) {
}


Vector3 DiffuseMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector2 uv) {
    return albedo.value(uv)*std::abs(odir.dot(n))*IPI;
}

SampleBSDF DiffuseMaterial::sample(Vector3 odir, Vector3 n, Sampler* sampler) {
    Sample3D sample = hemisphere.map(sampler->getSample2D());
    Vector3 idir = sample.value.rotate(n);
    return SampleBSDF(sample.value.rotate(n), Vector3(), sample.value.z, sample.pdf);
}

double DiffuseMaterial::getPdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return std::abs(dir.dot(n))*IPI;
}

