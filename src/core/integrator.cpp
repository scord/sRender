#include "integrator.h"
#include <cmath>

#include "material.h"
#include <chrono>
#include <assert.h>
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
        Vector3 lightEmission = lightInteraction.emission;

        if (lightInteractionP && lightEmission != Vector3()) {
            double cost2 = std::abs(lightN.dot(Vector3()-ray.direction.norm()));
            double lightRayLengthSquared = std::pow((lightInteraction.position - ray.origin).length(),2);
            double dist = (lightInteraction.position - ray.origin).length();
            double absorbsion = 1/(1+dist);
            double solidAngle = cost2*light->area / (lightRayLengthSquared);
            double lightPdf = 1/solidAngle;
            Interaction directInteraction(interaction.odir, ray.direction, interaction.position, interaction.uv, interaction.normal, interaction.material, true, lightPdf);
            double lightWeight = powerHeuristicDividedByPdf(directInteraction.pdf, interaction.pdf);
            lightContribution = lightEmission*absorbsion*directInteraction.brdf*directInteraction.cost*lightWeight;
        }

        if (nextInteractionP && nextInteractionP->emission != Vector3()) {
            double cost2 = std::abs(nextInteractionP->normal.dot(nextInteractionP->odir));
            double lightRayLength = (nextInteractionP->position - interaction.position).length();
  
            double absorbsion = 1/(1+lightRayLength);
            double solidAngle = cost2*light->area / (lightRayLength*lightRayLength);
            double lightPdf = 1/solidAngle;
            double brdfWeight = powerHeuristicDividedByPdf(interaction.pdf, lightPdf);
            brdfContribution = nextInteractionP->emission*absorbsion*interaction.brdf*interaction.cost*brdfWeight;
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
    Interaction* nextInteractionP = scene->intersect(ray);
    if (!nextInteractionP) {
        delete nextInteractionP;
        return bgColour;
    } 

    if (nextInteractionP->emission != Vector3()) {
        Vector3 emission = nextInteractionP->emission;
        delete nextInteractionP;
        return emission;
    }

    int i = 0;
    Vector3 totalEmission();

    while (i < depth && throughput != Vector3()) {
        interactionP = nextInteractionP;


        // sample new position on ray
        // if new position < interaction
        // create a medum interaction 
        double mediumPdf;
        double mediumT = scene->intersectMedium(*sampler, ray, mediumPdf);


        // sample brdf at interaction
        interactionP->sample(sampler);
        Interaction interaction = *interactionP;
        
        nextInteractionP = scene->intersect(interaction.getIncoming());

        assert(interaction.pdf >= 0);
        assert(interaction.cost >= 0);
        assert(interaction.brdf.x >= 0);

        double dist  = (interactionP->position - ray.origin).length();
        Vector3 emission = Vector3(1.0,0.8,0.8)*(dist)/10.0;
        
            

        if (interaction.pdf != 1 && DIRECT_LIGHT_SAMPLING) {
            // sample lights         
            // use multiple importance sampling if ray sampled from brdf also hits light

            colour += throughput*(getDirectIlluminationMIS(interactionP, nextInteractionP, scene, sampler) + emission);

            if (!nextInteractionP || nextInteractionP->emission != Vector3())
                i = depth;

            
            
            throughput =  throughput * interaction.brdf  * interaction.cost / interaction.pdf ;
        } else {
            double dist = (interaction.position - ray.origin).length();
            double absorbsion = 1/(1+dist);

            // don't directly sample lights if pdf is a delta function
            throughput = throughput * absorbsion * interaction.brdf  * interaction.cost / interaction.pdf;

            

            if (!nextInteractionP)
                i = depth;
            else if (nextInteractionP->emission != Vector3()) {
                colour += throughput*(nextInteractionP->emission + emission);
                i = depth;
            }
        }
 
        i++; 
        delete interactionP;  
    }

    delete nextInteractionP;

    return colour;
}
