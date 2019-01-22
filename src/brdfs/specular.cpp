#include "specular.h"

Ray SpecularBrdf::getRay(Ray ray, Vector3 p, Vector3 n) {
    double cosi = (Vector3() - ray.direction).dot(n);

    double etai = 1;
    double etat = 1.6;

    double fr = 1.0;

    // swap indices of refraction if exiting material
    if (cosi < 0.0) {
        std::swap(etai, etat);
        cosi = std::abs(cosi);
    }

    eta = etai/etat;

    // compute cost (cos(Theta))
    double sini = std::sqrt(std::max(0.0, 1 - cosi * cosi));
    double sint = eta * sini;
    double cost = std::sqrt(std::max(0.0, 1 - sint*sint));

    // if total internal reflection
    if (sint >= 1) {
        fr = 1.0;
    } else {
       
        double etaicost = etai*cost;
        double etatcosi = etat*cosi;
        double etaicosi = etai*cosi;
        double etatcost = etat*etat;

        // compute fresnel reflectance
        double rpara = ((etatcosi) - (etaicost)) /
                      ((etatcosi) + (etaicost));
        
        double rperp = ((etaicosi) - (etatcost)) /
                      ((etaicosi) + (etatcost));

        fr = (rpara * rpara + rperp * rperp) / 2.0;
    }

    Vector3 newDirection = Vector3();
    if (sample.value.x <= fr) {
        newDirection = (ray.direction - n*ray.direction.dot(n)*2).norm();
    } else {
        newDirection = ray.direction * eta + n*(eta*cosi - cost);
    }

    

    return Ray(p, newDirection);
}

Sample3D SpecularBrdf::getSample(double u1, double u2) {
    sample = Sample3D();
    sample.pdf = 1;
    sample.value.z = 1;
    sample.value.x = u1;
    sample.value.y = u2;

    return sample;
}

Vector3 SpecularBrdf::getValue(Vector3 albedo) {
    return Vector3(1,1,1)/(eta*eta);
}
