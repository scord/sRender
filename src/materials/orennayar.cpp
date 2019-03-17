#include "orennayar.h"

OrenNayarMaterial::OrenNayarMaterial() : Material(), hemisphere(Vector3(), 1) {
    roughness = 0.2;
}
OrenNayarMaterial::OrenNayarMaterial(Vector3 albedo) : Material(albedo, Vector3()), hemisphere(Vector3(), 1) {
    roughness = 0.2;
}

OrenNayarMaterial::OrenNayarMaterial(Texture& texture) : Material(), hemisphere(Vector3(), 1), texture(texture), roughness(0.2) {
    albedo.bindTexture(texture);
}

OrenNayarMaterial::OrenNayarMaterial(Vector3 albedo, Vector3 emission) : Material(albedo, emission), hemisphere(Vector3(), 1) {
}

OrenNayarMaterial::OrenNayarMaterial(Vector3 albedo, double roughness) : Material(albedo, Vector3()), hemisphere(Vector3(), 1), roughness(roughness) {
}


Vector3 OrenNayarMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector2 uv) {

    Vector3 notParallel(dir.z, dir.y, dir.x);
    Vector3 perp = (notParallel - n*notParallel.dot(n)).norm();
    Vector3 dirPerp = (dir - n*dir.dot(n)).norm();
    Vector3 odirPerp = (odir - n*odir.dot(n)).norm();
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

    return albedo.value(uv)*costi*(a+(b*c*sina*tanb))*IPI;
}

SampleBSDF OrenNayarMaterial::sample(Vector3 odir, Vector3 n, Sampler* sampler) {
    Sample3D sample = hemisphere.map(sampler->getSample2D());
    Vector3 idir = sample.value.rotate(n);
    return SampleBSDF(sample.value.rotate(n), Vector3(), sample.value.z, sample.pdf);
}

double OrenNayarMaterial::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {
    return std::abs(idir.dot(n))*IPI;
}


