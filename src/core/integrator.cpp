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


Sample3D Integrator::getDirectIllumination(Vector3 pos, Shape* object, Scene* scene, Disc* light) {
    Sample3D lightSample = light->sample(sampler.getRandomDouble(), sampler.getRandomDouble());

    Ray ray(pos, (lightSample.value - pos).norm());

    Vector3 closestLightIntersection;
    Shape* intersectedLight;
    Vector3 intersection;
    bool intersectionFound = scene->intersect(ray, closestLightIntersection, intersectedLight);


    Vector3 n = object->normal(pos);

    Vector3 lightN = light->normal(pos);

    double lightRayLength = ray.direction.length();
    double cost2 = std::abs(lightN.dot(Vector3()-ray.direction.norm()));
    double pdf = lightSample.pdf * lightRayLength*lightRayLength/cost2;
    if (intersectionFound && intersectedLight->isLight) {
        
        double cost = std::abs(ray.direction.dot(n)/(ray.direction.length()*n.length()));

        
        
        

        return Sample3D(intersectedLight->colour*object->brdf->getValue(object->colour)*cost, 1/TAU);
    } else {
        return Sample3D(Vector3(0,0,0), 0);
    }

}

double Integrator::balanceHeuristic(double pdf1, double pdf2) {
    return pdf1/(pdf1+pdf2);
}

double Integrator::balanceHeuristicDividedByPdf(double pdf, double pdf2) {
    return pdf/(pdf*pdf+pdf2*pdf2);
}

Vector3 Integrator::getRadiance(Ray ray, int depth, Scene* scene, Disc* light) {

    if (depth >= maxDepth) {
        return Vector3();
    }

    
    Vector3 closestIntersection;
    Shape* intersectedObject;


    Vector3 intersection;
    bool intersectionFound = scene->intersect(ray, closestIntersection, intersectedObject);
        
    if (!intersectionFound) {
        return Vector3(0,0,0);
    }
    if (intersectedObject->isLight) {
        return intersectedObject->colour;
    } else {

        Sample3D sample = intersectedObject->brdf->getSample(sampler.getRandomDouble(), sampler.getRandomDouble());

        

        Ray newRay = intersectedObject->brdf->getRay(ray, closestIntersection, intersectedObject->normal(closestIntersection));
        Vector3 brdf = intersectedObject->brdf->getValue(intersectedObject->colour);
        
        Vector3 radiance = getRadiance(newRay, depth+1, scene, light) * brdf*sample.value.z;

        if (sample.pdf == 1) {
            return radiance;
        }

        Sample3D directIlluminationSample = getDirectIllumination(closestIntersection, intersectedObject, scene, light);
        Vector3 directIllumination = directIlluminationSample.value*brdf;

        return radiance*balanceHeuristicDividedByPdf(sample.pdf, directIlluminationSample.pdf) + directIllumination*balanceHeuristicDividedByPdf(directIlluminationSample.pdf, sample.pdf);

    }

}
