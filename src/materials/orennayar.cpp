#include "orennayar.h"

OrenNayarMaterial::OrenNayarMaterial() : hemisphere(Vector3(), 1) {
    roughness = 0.2;
}
OrenNayarMaterial::OrenNayarMaterial(Vector3 albedo) : hemisphere(Vector3(), 1) {
    this->albedo = albedo;
    roughness = 0.2;
}
OrenNayarMaterial::OrenNayarMaterial(Vector3 albedo, Vector3 emission) : hemisphere(Vector3(), 1) {
    this->albedo = albedo;
    this->emission = emission;
}

OrenNayarMaterial::OrenNayarMaterial(Vector3 albedo, double roughness) : hemisphere(Vector3(), 1), roughness(roughness) {
    this->albedo = albedo;
}

Vector3 OrenNayarMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {

    Vector3 notParallel(dir.z, dir.y, dir.x);
    Vector3 perp = ((notParallel - n)*notParallel.dot(n)).norm();
    Vector3 dirPerp = ((dir - n)*dir.dot(n)).norm();
    Vector3 odirPerp = ((odir = n)*odir.dot(n)).norm();
    double cospi = std::abs(dirPerp.dot(perp));
    double cospr = std::abs(odirPerp.dot(perp));
    double costi = std::abs(dir.dot(n));
    double costr = std::abs(odir.dot(n));

    double sinpi = std::sqrt(std::max(0.0, 1.0 - cospi*cospi));
    double sinpr = std::sqrt(std::max(0.0, 1.0 - cospr*cospr));

    double sinti = std::sqrt(std::max(0.0, 1.0 - costi*costi));
    double sintr = std::sqrt(std::max(0.0, 1.0 - costr*costr));

    double sina = std::max(sinti, sintr);
    double tanb = std::min(sinti/costi, sintr/costr);

    double c = std::max(0.0, cospi*cospr + sinpi*sinpr);

    double r2 = roughness*roughness;
    double a = 1 - 0.5*r2/(r2 + 0.33);
    double b = 0.45*r2/(r2 + 0.09);

    return albedo*costi*(a+(b*c*sina*tanb))*IPI;
}

Sample3D OrenNayarMaterial::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {
    Sample3D sample = hemisphere.map(sampler->getSample2D());
   // Vector3 test = sample.value.rotationMatrix()*n ;

    return Sample3D(sample.value.rotate(n), sample.pdf);
}

double OrenNayarMaterial::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {
    return std::abs(idir.dot(n))*IPI;
}
