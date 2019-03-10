#include "material.h"

Material::Material() : albedo(0.9,0.9,0.9), emission(0,0,0) {}

Vector3 Material::reflectance(Vector3 idir, Vector3 odir, Vector3 n) {
    return getBrdf(idir, odir, n)/getPdf(idir, odir, n);
}

Vector3 Material::getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector3 p) {
    return getBrdf(dir, odir, n);
}

Vector3 Material::getAlbedo(Vector3 p) {
    return albedo;
};

void Material::bind(Shape* geometry) {
    uvMapping = geometry->defaultUVMapping();
}
