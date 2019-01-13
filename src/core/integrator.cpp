#include "integrator.h"
#include <cmath>
#include "sampler.h"
#include "brdf.h"
#include <chrono>
std::mt19937 mt;
std::uniform_real_distribution<double> uniform;

Integrator::Integrator(PixelSampler& sampler, int maxDepth) {
   this->maxDepth = maxDepth;
   this->sampler = sampler;
}


Vector3 Integrator::getRadiance(Ray ray, int depth, Scene scene, Disc light) {

    if (depth >= maxDepth) {
        
        Sample3D lightSample = light.sample(sampler.getRandomDouble(), sampler.getRandomDouble());

        ray.direction = lightSample.value - ray.origin;

        Vector3 closestIntersection;
        Shape* intersectedObject;
        Vector3 intersection;
        bool intersectionFound = scene.intersect(ray, closestIntersection, intersectedObject);

        if (!intersectionFound) {
            return Vector3(2000,2000,2000);
        }
        else if (intersectedObject->isLight) {
            return intersectedObject->colour;
        } else {
            return Vector3(0,0,0);
        }

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

    Sample3D sample = sampler.getRandomHemisphereSample();
    

    intersectedObject->brdf->giveSample(sample);

    Vector3 brdf = intersectedObject->brdf->getValue(intersectedObject->colour);

    Ray newRay = intersectedObject->brdf->getRay(ray, closestIntersection, intersectedObject->normal(closestIntersection));

    // RUSSIAN ROULETTE PATH TERMINATION
    double e = sampler.getRandomDouble();
    double p = 0.9*std::fmax(intersectedObject->colour.x,
                        std::fmax(intersectedObject->colour.y,
                                  intersectedObject->colour.z));
    if (e > p ) {
        Sample3D lightSample = light.sample(sampler.getRandomDouble(), sampler.getRandomDouble());

        ray.direction = lightSample.value - ray.origin;

        Vector3 closestIntersection;
        Shape* intersectedObject;
        Vector3 intersection;
        bool intersectionFound = scene.intersect(ray, closestIntersection, intersectedObject);

        if (!intersectionFound) {
            return Vector3(2000,2000,2000);
        }
        else if (intersectedObject->isLight) {
            return intersectedObject->colour;
        } else {
            return Vector3(0,0,0);
        }
    }

    return getRadiance(newRay, depth+1, scene, light) * 0.4*brdf / (sample.pdf/sample.value.z);

}