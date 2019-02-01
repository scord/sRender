#include "specular.h"

double SpecularBrdf::fresnel(double cosi, double etai, double etat) {


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

Ray SpecularBrdf::getRay(Ray ray, Vector3 p, Vector3 n) {
    double cosi = ray.direction.norm().dot(n);
    double fr = fresnel(cosi,  1, 2.4);

    double etai = 1;
    double etat = 2.4;

    if (cosi < 0.0) {
        cosi = -cosi;
    } else {
        n = Vector3() - n;
        std::swap(etai, etat);
    }

    eta = etai/etat;
    double k = 1 - eta*eta * (1 - cosi*cosi);
   
 

    Vector3 newDirection = Vector3();
    if (sample.value.x <= fr) {
        newDirection = (ray.direction - n*ray.direction.dot(n)*2).norm();
        eta = 1;
    } else {
        double sin2i = std::max(0.0, 1.0-cosi*cosi);
        double sin2t = eta*eta*sin2i;
        double cost = std::sqrt(std::max(0.0, 1 - sin2t));
        newDirection = ray.direction*eta + n*(eta*cosi - cost);
    }

    

    return Ray(p, newDirection);
}

Sample3D SpecularBrdf::getSample(double u1, double u2) {
    sample = Sample3D();
    sample.pdf = 0;
    sample.value.z = 1;
    sample.value.x = u1;
    sample.value.y = u2;

    return sample;
}

Vector3 SpecularBrdf::getValue(Vector3 albedo) {
    return Vector3(1,1,1);
}


double SpecularBrdf::getPdf(Ray ray, Vector3 p, Vector3 n) {
    return 0;
}
