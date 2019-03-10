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
    
    delete lightInteractionP;

    double lightRayLength = std::abs((lightInteraction.position - ray.origin).length());
    double costl = std::abs(lightInteraction.normal.dot(Vector3()-ray.direction.norm()));

    Vector3 brdf = interaction.getBrdf();
    double brdfPdf = interaction.getPdf(); 

    double cost = std::abs(ray.direction.dot(interaction.normal));

    double solidAngle = costl / (lightRayLength*lightRayLength*lightSample.pdf);
    lightPdf = 1/solidAngle;

    interaction.forceSample(ray.direction, lightPdf);

    return lightInteraction.material->emission*interaction.brdf*interaction.cost/interaction.pdf;   
}

Vector3 Integrator::getDirectIlluminationMIS(Interaction* interactionP, Interaction* nextInteractionP, Scene* scene, Sampler* sampler) {    
    
    Interaction interaction = *interactionP;
    Vector3 n = interaction.normal;
    Vector3 totalContribution;
    
    for (auto l : scene->lights) {

        Disc* light = static_cast<Disc*>(l->geometry[0]);
        Sample3D lightSample = light->sample(sampler->getRandomDouble(), sampler->getRandomDouble());

        Ray ray(interaction.position, (lightSample.value - interaction.position).norm());

        Interaction* lightInteractionP = scene->intersect(ray);
        Interaction lightInteraction = *lightInteractionP;
        Vector3 lightN = lightInteraction.normal;
        Vector3 lightContribution = Vector3();
        Vector3 brdfContribution = Vector3();    
        Vector3 lightEmission = lightInteraction.material->emission;

        if (lightInteractionP && lightEmission != Vector3()) {
            double cost2 = std::abs(lightN.dot(Vector3()-ray.direction.norm()));
            double lightRayLengthSquared = std::pow((lightInteraction.position - ray.origin).length(),2);
            double solidAngle = cost2*light->area / (lightRayLengthSquared);
            double lightPdf = 1/solidAngle;

            Interaction directInteraction(interaction.odir, ray.direction, interaction.position, interaction.normal, interaction.material, true, lightPdf);
            double lightWeight = powerHeuristicDividedByPdf(directInteraction.pdf, interaction.pdf);
            lightContribution = lightEmission*directInteraction.brdf*directInteraction.cost*lightWeight;
        }

        if (nextInteractionP && nextInteractionP->material->emission != Vector3()) {
            double cost2 = std::abs(nextInteractionP->normal.dot(nextInteractionP->odir));
            double lightRayLength = (nextInteractionP->position - interaction.position).length();
            double solidAngle = cost2*light->area / (lightRayLength*lightRayLength);
            double lightPdf = 1/solidAngle;
            double brdfWeight = powerHeuristicDividedByPdf(interaction.pdf, lightPdf);
            brdfContribution = nextInteractionP->material->emission*interaction.brdf*interaction.cost*brdfWeight;
        }

        totalContribution += lightContribution + brdfContribution;

        delete lightInteractionP;
    }
    
    return totalContribution;
}

Vector3 Integrator::getRadiance(Ray ray, int depth, Scene* scene, Sampler* sampler) {

    Vector3 throughput = Vector3(1,1,1);
    Vector3 colour = Vector3(0,0,0);
    Vector3 bgColour = Vector3(0,0,0);
    int n_lights = scene->lights.size();
    const bool DIRECT_LIGHT_SAMPLING = true;
    Interaction* interactionP = nullptr;
    Interaction* nextInteractionP = scene->intersect(ray);;
    if (!nextInteractionP) {
        delete nextInteractionP;
        return bgColour;
    } 

    if (nextInteractionP->material->emission != Vector3()) {
        Vector3 emission = nextInteractionP->material->emission;
        delete nextInteractionP;
        return emission;
    }

    int i = 0;
    while (i < depth && throughput != Vector3()) {
        interactionP = nextInteractionP;

        // sample brdf at interaction
        interactionP->sample(sampler);
        Interaction interaction = *interactionP;
        
        nextInteractionP = scene->intersect(interaction.getIncoming());

        if (interaction.pdf != 1 && DIRECT_LIGHT_SAMPLING) {
            // sample lights         
            // use multiple importance sampling if ray sampled from brdf also hits light
            colour += throughput*getDirectIlluminationMIS(interactionP, nextInteractionP, scene, sampler);

            if (!nextInteractionP || nextInteractionP->material->emission != Vector3())
                i = depth;
         
            throughput = throughput * interaction.brdf * interaction.cost / interaction.pdf;
        } else {
            // don't directly sample lights if pdf is a delta function
            throughput = throughput * interaction.brdf * interaction.cost / interaction.pdf;

            if (!nextInteractionP)
                i = depth;
            else if (nextInteractionP->material->emission != Vector3()) {
                colour += throughput*nextInteractionP->material->emission;
                i = depth;
            }
        }
 
        i++; 
        delete interactionP;  
    }

    delete nextInteractionP;
    return colour;
}
