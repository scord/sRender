#include "specular.h"

double SpecularMaterial::fresnel(double cosi, double etai, double etat) {

    double fr = 1.0;

    // swap indices of refraction if exiting material

    if (cosi > 0) {
        std::swap(etai, etat);
    } else {
        cosi = -cosi;
    }
    
    double eta = etai/etat;

    // compute cost (cos(Theta))

    double sin2i = std::max(0.0, 1.0 - cosi*cosi);
    double sini = std::sqrt(sin2i);

    double sin2t = eta*eta*sin2i;
    double sint = eta * sini;
    double cost = std::sqrt(std::max(0.0, 1 - sint*sint));

    // if total internal reflection
    if (sint >= 1) {
        return 1.0;
    } else {

        cosi = std::abs(cosi);

        std::swap(etai, etat);
        std::swap(cosi, cost);

        double etaicost = etai*cost;
        double etatcosi = etat*cosi;
        double etaicosi = etai*cosi;
        double etatcost = etat*etat;

        // compute fresnel reflectance
        double rpara = ((etatcosi) - (etaicost)) /
                      ((etatcosi) + (etaicost));
        
        double rperp = ((etaicosi) - (etatcost)) /
                      ((etaicosi) + (etatcost));

        return (rpara * rpara + rperp * rperp) / 2.0;
    }
}

SpecularMaterial::SpecularMaterial() : Material() {
}
SpecularMaterial::SpecularMaterial(vec3 albedo) : Material(albedo, vec3()){
}

vec3 SpecularMaterial::getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv) {
    return albedo.value(uv);
}

SampleBSDF SpecularMaterial::sample(vec3 dir, vec3 n, Sampler& sampler) {
    dir = vec3() - dir;
    double cosi = dir.dot(n);

    double etai = 1;
    double etat = 1.5;

    double fr = fresnel(cosi, etai, etat);

    

    if (cosi < 0.0) {
        cosi = -cosi;
    } else {
        n = vec3() - n;
        std::swap(etai, etat);
    }

    double eta = etai/etat;
    double k = 1 - eta*eta * (1 - cosi*cosi);
   
    vec3 newDirection = vec3();
    if (sampler.getRandomDouble() <= fr) {
        newDirection = (dir - n*dir.dot(n)*2).norm();
        eta = 1;
    } else {
        double sin2i = std::max(0.0, 1.0-cosi*cosi);
        double sin2t = eta*eta*sin2i;
        double cost = std::sqrt(std::max(0.0, 1 - sin2t));
        newDirection = dir*eta + n*(eta*cosi - cost);
    }

    return SampleBSDF(newDirection, vec3(), 1, 1);
}

double SpecularMaterial::getPdf(vec3 dir, vec3 odir, vec3 n) {
    return 0;
}

