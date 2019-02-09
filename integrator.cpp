#include "integrator.h"
#include <cmath>

#include "material.h"
#include <chrono>
std::mt19937 mt;
std::uniform_real_distribution<double> uniform;

Integrator::Integrator(Sampler& sampler, int maxDepth) {
   this->maxDepth = maxDepth;
   this->sampler = sampler;
}


Vector3 Integrator::getDirectIllumination(Vector3 pos, Shape* object, Scene* scene, Disc* light) {
    Sample3D lightSample1 = light->sample(sampler.getRandomDouble(), sampler.getRandomDouble());

    Ray ray(pos, (lightSample1.value - pos).norm());

    Vector3 closestLightIntersection;
    Shape* intersectedLight;

    Interaction* interaction = scene->intersect(ray);

    Vector3 n = object->normal(pos);
    Vector3 lightContribution = Vector3();
    Vector3 lightN = light->normal(pos);
    Sample3D lightSample;
    bool missedLight = false;
    double lightWeight = 0;
    double lightRayLength = std::abs((closestLightIntersection - ray.origin).length());
    double cost2 = std::abs(lightN.dot(Vector3()-ray.direction.norm()));
    double pdf = lightSample1.pdf;
    if (intersectionFound && intersectedLight->isLight) {
        double cost = std::abs(ray.direction.dot(n));
        double solidAngle = cost2 / (lightRayLength*lightRayLength*pdf);
        pdf = 1/solidAngle;
     
        double scatteringPdf = object->brdf->getPdf(ray, pos, n);
        missedLight = false;
       
        lightWeight = balanceHeuristicDividedByPdf(pdf, scatteringPdf);
        lightContribution = intersectedLight->colour*object->brdf->getValue(object->colour)*2*PI*cost;
    
        
    } else {
        lightContribution = Vector3();
        missedLight = true;
    }

    Vector3 closestIntersection;
    Sample3D sample = object->brdf->getSample(sampler.getRandomDouble(), sampler.getRandomDouble());
    ray = object->brdf->getRay(ray, pos, object->normal(pos));

    double brdfPdf = object->brdf->getPdf(ray, pos, object->normal(pos)); 

    Vector3 brdf = object->brdf->getValue(object->colour);


    Shape* intersectedObject;

    Vector3 brdfContribution = Vector3();
    intersectionFound = scene->intersect(ray, closestIntersection, intersectedLight);
    Sample3D brdfSample;
    double brdfWeight = 0;
    if (intersectionFound && intersectedLight->isLight) {
        double cost2 = std::abs(lightN.dot(Vector3()-ray.direction.norm()));
        double lightRayLength = (closestIntersection - ray.origin).length();
        double solidAngle = cost2 / (lightRayLength*lightRayLength*lightSample1.pdf);
        double lightPdf = 1/solidAngle;
        double cost = std::abs(ray.direction.dot(n)/(ray.direction.length()*n.length()));
        brdfWeight = balanceHeuristicDividedByPdf(brdfPdf, lightPdf);

        brdfContribution = intersectedLight->colour*object->brdf->getValue(object->colour)*sample.value.z;
        
    } else {
        brdfContribution = Vector3(0,0,0);
    }

    if (pdf == 0) {
        return Vector3();
    }
    return lightContribution*lightWeight + brdfContribution*brdfWeight;

}

double Integrator::balanceHeuristic(double pdf1, double pdf2) {
    return pdf1/(pdf1+pdf2);
}

double Integrator::balanceHeuristicDividedByPdf(double pdf, double pdf2) {
    float test = 1;
    return pdf/(pdf*pdf+pdf2*pdf2);
}

Vector3 Integrator::getRadiance(Ray ray, int depth, Scene* scene, Disc* light) {

    Vector3 throughput = Vector3(1,1,1);
    Vector3 colour = Vector3(0,0,0);

    Sample3D sample = Sample3D();
    sample.pdf = 1;
    bool hitLight = false;
    double brdfPdf = 0; 
    double samples = 1;

    for (int i = 0; i < 8; i++) {

        Vector3 closestIntersection;
        Shape* intersectedObject;

        Vector3 intersection;
        Interaction* interaction = scene->intersect(ray);
        
        if (!interaction) {
            return Vector3();
        }
        else if (intersectedObject->isLight) {
            if (!hitLight) {
                if (i == 0 ) {
                    colour = intersectedObject->colour;
                    break;
                } else if (brdfPdf == 0) {
                    colour = colour + throughput*intersectedObject->colour;
                    break;
                } 
            } else {
                break;
            }
        } else if (!intersectedObject->isLight){
            if (i == 0) {
                
                Sample2D stratified = sampler.getStratifiedSample();
                sample = intersectedObject->brdf->getSample(sampler.getRandomDouble(), sampler.getRandomDouble());
            } else {
                sample = intersectedObject->brdf->getSample(sampler.getRandomDouble(), sampler.getRandomDouble());
            }
            ray = intersectedObject->brdf->getRay(ray, closestIntersection, intersectedObject->normal(closestIntersection));
            Vector3 brdf = intersectedObject->brdf->getValue(intersectedObject->colour);
            brdfPdf = intersectedObject->brdf->getPdf(ray, closestIntersection, intersectedObject->normal(closestIntersection)); 
      
            if (brdfPdf != 0) {
                
                Vector3 directLighting = getDirectIllumination(closestIntersection, intersectedObject, scene, light);
                samples++;
                
                if (directLighting != Vector3(0,0,0)) {
                    hitLight = true;
                    colour = colour + throughput*directLighting;
                    
                } else {
                    hitLight = false;
                }
            }


            


            if (brdfPdf == 0) {
                throughput = throughput * brdf;
            } else {
                throughput = throughput * brdf*sample.value.z/brdfPdf;
            }

        } 
    }

    return colour/samples;
}
