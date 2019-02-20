#include "material.h"

Material::Material() : albedo(0.9,0.9,0.9), emission(0,0,0) {}

Vector3 Material::reflectance(Vector3 idir, Vector3 odir, Vector3 n) {
    return getBrdf(idir, odir, n)/getPdf(idir, odir, n);
}

Sample3D Material::sample(Vector3 dir, Vector3 odir, Vector3 n, Vector3& reflectance, Sampler* sampler) {
    return sample(dir, odir, n, sampler);
}