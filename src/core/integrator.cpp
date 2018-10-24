#include "integrator.h"
#include <cmath>
#include "../samplers/pixelsampler.h"
#include "brdf.h"

std::mt19937 mt;
std::uniform_real_distribution<double> uniform;

Integrator::Integrator() {
   maxDepth = 15;
}

Vector3 Integrator::getRadiance(Ray ray, int depth, std::vector<Shape*> scene) {

    if (depth >= maxDepth) {
        return Vector3();
    }
    
    Vector3 closestIntersection;
    Shape* intersectedObject;

    bool intersectionFound = false;
    double tmin = 99999;
    for (Shape* object : scene) {
        Vector3 intersection;
        
        double t = object->Intersect(ray, intersection);
        
        if (t < tmin && t > 0) {
            tmin = t;
            closestIntersection = intersection;
            intersectedObject = object;
            intersectionFound = true;
        }
    }

    if (!intersectionFound) {
        return Vector3();
    }
    if (intersectedObject->isLight) {
        return intersectedObject->colour;
    }

    Ray newRay = intersectedObject->brdf->Sample(ray, closestIntersection, intersectedObject->normal(closestIntersection));

    // RUSSIAN ROULETTE PATH TERMINATION
    double e = uniform(mt);
    double p = 0.9*std::fmax(intersectedObject->colour.x,
                        std::fmax(intersectedObject->colour.y,
                                  intersectedObject->colour.z));
    if (e > p ) {
        return Vector3();
    }

    return getRadiance(newRay, depth+1, scene) * intersectedObject->colour / (intersectedObject->brdf->pdf * p);

}