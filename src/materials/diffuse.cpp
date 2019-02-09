#include "diffuse.h"

DiffuseMaterial::DiffuseMaterial() : hemisphere(Vector3(), 1) {
}
DiffuseMaterial::DiffuseMaterial(Vector3 albedo) : hemisphere(Vector3(), 1) {
    this->albedo = albedo;
}
DiffuseMaterial::DiffuseMaterial(Vector3 albedo, Vector3 emission) : hemisphere(Vector3(), 1) {
    this->albedo = albedo;
    this->emission = emission;
}

Vector3 DiffuseMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return albedo*std::abs(dir.dot(n))*IPI;
}

Sample3D DiffuseMaterial::sample(Vector3 dir, Vector3 odir, Vector3 n, Sampler* sampler) {
    Sample3D sample = hemisphere.map(sampler->getSample2D());
   // Vector3 test = sample.value.rotationMatrix()*n ;

    return Sample3D(sample.value.rotate(n), sample.pdf);
}

double DiffuseMaterial::getPdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return std::abs(dir.dot(n))*IPI;
}

