#include "integrator.h"
#include <cmath>

#include "material.h"
#include <chrono>
std::mt19937 mt;
std::uniform_real_distribution<double> uniform;

Integrator::Integrator(int maxDepth) {
   this->maxDepth = maxDepth;
}

double Integrator::balanceHeuristic(double pdf1, double pdf2) {
    return pdf1/(pdf1+pdf2);
}

double Integrator::powerHeuristicDividedByPdf(double pdf, double pdf2) {
    return pdf/(pdf*pdf+pdf2*pdf2);
}

Vector3 Integrator::getDirectIllumination(Interaction* interactionP, Scene* scene, Disc* light, Sampler* sampler) {
    Sample3D lightSample = light->sample(sampler->getRandomDouble(), sampler->getRandomDouble());

    Interaction interaction = *interactionP;
  
    Ray ray(interaction.position, (lightSample.value - interaction.position).norm());

    Interaction* lightInteractionP = scene->intersect(ray);
    Interaction lightInteraction = *lightInteractionP;

    double pdf = lightInteraction.getPdf();
    double lightPdf = 1;
    
    if (lightInteractionP) {
        Vector3 lightEmission = lightInteraction.material->emission;
        if (lightEmission != Vector3()) {
        
            delete lightInteractionP;

            double lightRayLength = std::abs((lightInteraction.position - ray.origin).length());
            double cost2 = std::abs(lightInteraction.normal.dot(Vector3()-ray.direction.norm()));

            Vector3 brdf = interaction.getBrdf();
            double brdfPdf = interaction.getPdf(); 

            double cost = std::abs(ray.direction.dot(interaction.normal));
            double solidAngle = cost2*light->area/ (lightRayLength*lightRayLength);
            lightPdf = 1/solidAngle;
            interaction.idir = ray.direction;

            if (lightPdf == 0) {
                return Vector3();
            } else {
                return lightEmission*interaction.getBrdf()/lightPdf;   
            }
        } else {
            delete lightInteractionP;
            return Vector3();
        } 
    } else {
        delete lightInteractionP;
        return Vector3();
    }
   
}

Vector3 Integrator::getDirectIlluminationMIS(Interaction* interactionP, Interaction* nextInteractionP, Scene* scene, Disc* light, Sampler* sampler) {    
    
    Sample3D lightSample = light->sample(sampler->getRandomDouble(), sampler->getRandomDouble());
    Ray ray(interactionP->position, (lightSample.value - interactionP->position).norm());

    Interaction* lightInteractionP = scene->intersect(ray);
    Interaction lightInteraction = *lightInteractionP;
    delete lightInteractionP;
    
    Vector3 n = interactionP->normal;
    Vector3 lightN = lightInteraction.normal;

    Vector3 lightContribution = Vector3();
    double lightWeight = 0;
    double lightPdf = 0;

    Vector3 brdfContribution = Vector3();
    double brdfWeight = 0;
    double brdfPdf = 0; 

    
    
    Vector3 lightEmission = lightInteraction.material->emission;

    

    if (lightEmission != Vector3()) {
       
        double cost2 = std::abs(lightN.dot(Vector3()-ray.direction.norm()));
        double lightRayLengthSquared = std::pow((lightInteraction.position - ray.origin).length(),2);
        double solidAngle = cost2*light->area / (lightRayLengthSquared);
        lightPdf = 1/solidAngle;

        Interaction directInteraction(interactionP->odir, ray.direction, interactionP->position, interactionP->normal, interactionP->material, true);

        lightWeight = powerHeuristicDividedByPdf(lightPdf, directInteraction.getPdf());
        lightContribution = lightEmission*directInteraction.getBrdf();
    } 
    
    if (nextInteractionP && nextInteractionP->material->emission != Vector3()) {

        double cost2 = std::abs(nextInteractionP->normal.dot(nextInteractionP->odir));
        double lightRayLength = (nextInteractionP->position - interactionP->position).length();
        double solidAngle = cost2*light->area / (lightRayLength*lightRayLength);
        double lightPdf2 = 1/solidAngle;

        brdfPdf = interactionP->getPdf();
        brdfWeight = powerHeuristicDividedByPdf(brdfPdf, lightPdf2);

        brdfContribution = nextInteractionP->material->emission*interactionP->getBrdf();
    }

    if (lightContribution != Vector3() && brdfContribution != Vector3()) {
        return lightContribution*lightWeight + brdfContribution*brdfWeight;
    } else if (lightContribution != Vector3()) {
        return lightContribution/lightPdf;
    } else if (brdfContribution != Vector3()) {
        return brdfContribution/brdfPdf;
    } else {
        return Vector3();
    }
}

Vector3 Integrator::getRadiance(Ray ray, int depth, Scene* scene, Disc* light, Sampler* sampler) {

    Vector3 throughput = Vector3(1,1,1);
    Vector3 colour = Vector3(0,0,0);

    double samples = 0;
    bool hitLight = false;
    Interaction* interactionP = scene->intersect(ray);
    Interaction* nextInteractionP = nullptr;
    if (!interactionP) {
        delete interactionP;
        return Vector3();
    } 

    for (int i = 0; i < 5; i++) {

        interactionP->sample(sampler);
        Vector3 emission = interactionP->material->emission;

        if (emission != Vector3() && !hitLight) {
            colour = throughput*emission;
            samples++;
            break;
        }
        hitLight = false;
        
        ray = interactionP->getIncoming();

        nextInteractionP = scene->intersect(ray);
        
        Vector3 directLighting = Vector3();
        double pdf = interactionP->getPdf();
  

        if (nextInteractionP && emission == Vector3() && interactionP->getPdf() != 0 && nextInteractionP->material->emission != Vector3())
            directLighting = getDirectIlluminationMIS(interactionP, nextInteractionP, scene, light, sampler);
        else if (emission == Vector3() && interactionP->getPdf() != 0)
            directLighting = getDirectIllumination(interactionP, scene, light, sampler);

    
        if (nextInteractionP && nextInteractionP->material->emission != Vector3() && directLighting != Vector3()) {
            colour = colour + throughput*directLighting;
            hitLight = true;
            samples++;
            delete nextInteractionP;
            break;
        } else if (directLighting != Vector3(0,0,0)) {  
            
            samples++;
            colour = colour + throughput*directLighting;
            hitLight = true;
        }

        if (pdf != 0) {
            throughput = throughput * interactionP->getBrdf() / pdf;
        } else {
            i--;
            throughput = throughput * interactionP->getBrdf();
        }

        if (throughput == Vector3()) {
            delete nextInteractionP;
            break;
        }

        if (nextInteractionP) {  
            delete interactionP;    
            interactionP = nextInteractionP;
        } else {
            break;
        }  
    }

    delete interactionP;
    
    if (samples == 0)
        return Vector3();

    return colour;
}
