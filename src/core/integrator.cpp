#include "integrator.h"
#include <cmath>
#include "../samplers/pixelsampler.h"
#include "brdf.h"

std::mt19937 mt;
std::uniform_real_distribution<double> uniform;

Integrator::Integrator() {
   maxDepth = 10;
}

Vector3 Integrator::getRadiance(Ray ray, int depth, Scene scene) {

    if (depth >= maxDepth) {
        return Vector3();
    }
    
    Vector3 closestIntersection;
    Shape* intersectedObject;


    Vector3 intersection;
    bool intersectionFound = scene.intersect(ray, closestIntersection, intersectedObject);
        
       

    if (!intersectionFound) {
        return Vector3(2000,2000,2000);
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

    return getRadiance(newRay, depth+1, scene) * intersectedObject->colour / (intersectedObject->brdf->pdfDividedByCos * p);

}