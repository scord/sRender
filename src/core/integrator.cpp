#include "integrator.h"
#include <cmath>

#include "material.h"
#include "tonemapper.h"
#include <chrono>
#include <assert.h>

Integrator::Integrator(int maxDepth, const Scene& scene, Sampler& sampler) {
   this->maxDepth = maxDepth;
   this->scene = scene;
   this->sampler = sampler;
   this->media = scene.medium->scattering.x > 0.0 && scene.medium->scattering.y > 0.0 && scene.medium->scattering.z;
   this->direct_light_sampling = true;
}

double Integrator::balanceHeuristic(double pdf1, double pdf2) {
    return pdf1/(pdf1+pdf2);
}

double Integrator::powerHeuristicDividedByPdf(double pdf, double pdf2) {
    return pdf/(pdf*pdf+pdf2*pdf2);
}

vec3 Integrator::getDirectIllumination(const MediumInteraction& interaction, Sample3D lightSample) {    
    

    vec3 totalContribution;
    
    Ray ray(interaction.position, (lightSample.value - interaction.position).norm());

    Interaction lightInteraction = scene.intersect(ray);


    vec3 lightN = lightInteraction.normal;
    vec3 lightContribution = vec3();
    vec3 lightEmission = lightInteraction.emission;

    if (lightInteraction.hit && lightEmission != vec3()) {
        double cost2 = std::abs(lightN.dot(vec3()-ray.direction.norm()));
        double lightRayLength = (lightInteraction.position - ray.origin).length();
        double lightRayLengthSquared = std::pow(lightRayLength,2);
        double solidAngle = cost2 / (lightRayLengthSquared);
        double lightPdf = lightSample.pdf/solidAngle;

    
        lightContribution = lightEmission*scene.medium->transmittance(lightRayLength)/(lightPdf);
    }


    totalContribution += lightContribution;


    
    return totalContribution;
}

vec3 Integrator::getDirectIllumination(const Interaction& interaction) {    
    

    vec3 totalContribution;
    
    for (auto l : scene.lights) {

        Shape* light = l->geometry[0];
        Sample3D lightSample = light->sample(sampler.getRandomDouble(), sampler.getRandomDouble());
        
     
        Ray ray(interaction.position, (lightSample.value - interaction.position).norm());

        Interaction lightInteraction = scene.intersect(ray);


        vec3 lightN = lightInteraction.normal;
        vec3 lightContribution = vec3();
    
        vec3 lightEmission = lightInteraction.emission;

        if (lightInteraction.hit && lightEmission != vec3()) {
            double cost2 = std::abs(lightN.dot(vec3()-ray.direction.norm()));
            double lightRayLength = (lightInteraction.position - ray.origin).length();
            double lightRayLengthSquared = std::pow(lightRayLength,2);
            double solidAngle = cost2 / (lightRayLengthSquared);
            double lightPdf = lightSample.pdf/solidAngle;

            Interaction directInteraction(interaction.odir, ray.direction, interaction.position, interaction.uv, interaction.normal, interaction.material, true, lightPdf);
           
            lightContribution = lightEmission*directInteraction.brdf* scene.medium->transmittance(lightRayLength)/directInteraction.pdf;
        }

   

        totalContribution += lightContribution;

    }
    
    return totalContribution;
}

vec3 Integrator::getDirectIllumination(const Interaction& interaction, const Interaction& nextInteraction) {    
    

    vec3 totalContribution;
    
    for (auto l : scene.lights) {

        Shape* light = l->geometry[0];
        Sample3D lightSample = light->sample(sampler.getRandomDouble(), sampler.getRandomDouble());
        
     
        Ray ray(interaction.position, (lightSample.value - interaction.position).norm());

        Interaction lightInteraction = scene.intersect(ray);


        vec3 lightN = lightInteraction.normal;
        vec3 lightContribution = vec3();
        vec3 brdfContribution = vec3();    
        vec3 lightEmission = lightInteraction.emission;

        if (lightInteraction.hit && lightEmission != vec3()) {
            double cos = std::abs(lightN.dot(vec3()-ray.direction.norm()));
            double lightRayLength = (lightInteraction.position - ray.origin).length();
            double lightRayLengthSquared = lightRayLength*lightRayLength;
            double solidAngle = cos / (lightRayLengthSquared);
            double lightPdf = lightSample.pdf/solidAngle;

            Interaction directInteraction(interaction.odir, ray.direction, interaction.position, interaction.uv, interaction.normal, interaction.material, true, lightPdf);
            double lightWeight = powerHeuristicDividedByPdf(directInteraction.pdf, interaction.pdf);
            lightContribution = lightEmission*directInteraction.brdf* scene.medium->transmittance(lightRayLength)*lightWeight;
        }

  
        double cos = std::abs(nextInteraction.normal.dot(nextInteraction.odir));
        double lightRayLength = (nextInteraction.position - interaction.position).length();
        double solidAngle = cos / (lightRayLength*lightRayLength);
        double lightPdf = lightSample.pdf/solidAngle;
        double brdfWeight = powerHeuristicDividedByPdf(interaction.pdf, lightPdf);
        brdfContribution = nextInteraction.emission*interaction.brdf* scene.medium->transmittance(lightRayLength)*brdfWeight;
        
        totalContribution += lightContribution + brdfContribution;
    }
    
    return totalContribution;
}

Sample<Object*> Integrator::chooseLight() {
    double u = sampler.getRandomDouble();

    double n = scene.lights.size();

    double m = int(u*n);

    return Sample<Object*>(scene.lights[m], 1/n);
}


Sample3D Integrator::sampleLights() {
    Sample<Object*> light = chooseLight();

    Sample3D lightSample = light.value->geometry[0]->sample(sampler.getRandomDouble(), sampler.getRandomDouble());

    lightSample.pdf *= light.pdf;

    return lightSample;
}

/* Sample3D Integrator::sampleLights(const vec3& pos) {
    double l = sampler.getRandomDouble();
    double u = sampler.getRandomDouble();
    double v = sampler.getRandomDouble();

    double totalSolidAngle = 0;
    std::vector<double> accumSolidAngles;
    std::vector<double> solidAngles;

    for (auto& lightObject : scene.lights) {
        Disc* light = static_cast<Disc*>(lightObject->geometry[0]);

        vec3 dir = pos - light->p0;

        double cos = std::abs(light->n.dot(dir));

        double len = dir.length();


        double solidAngle = std::max(0.0, cos*light->area / (len*len));

        solidAngles.push_back(solidAngle);
        totalSolidAngle += solidAngle;
        accumSolidAngles.push_back(totalSolidAngle);
    }

    Disc* light;
    double pdf = 0;

    if (totalSolidAngle <= 0) {
        light = static_cast<Disc*>(scene.lights[0]->geometry[0]);
        Sample3D lightSample = light->sample(u,v);

        return Sample3D(lightSample.value, lightSample.pdf);
    }

    for (int i = 0; i < accumSolidAngles.size(); i++) {
        double accumSolidAngle = accumSolidAngles[i];
    
        double p = accumSolidAngle/totalSolidAngle;
        
        if (l < p) {
            light = static_cast<Disc*>(scene.lights[i]->geometry[0]);
            pdf = solidAngles[i]/totalSolidAngle;
            break;
        }
    }

    Sample3D lightSample = light->sample(u,v);

    return Sample3D(lightSample.value, lightSample.pdf*pdf);
    

} */

vec3 Integrator::getRadiance(Ray ray, int depth) {

    vec3 throughput = vec3(1,1,1);
    vec3 colour = vec3(0,0,0);

    Interaction interaction;
      
    interaction = scene.intersect(ray);

    
    for (int i = 0; i < depth; i++) {

        // get a random light from the scene and sample a point on the light
        Sample3D lightSample = sampleLights();

        // get the length of the ray
        double rayLength = (interaction.position - ray.origin).length();   

        // sample along the ray with equiangular sampling
        Sample<double> raySample = ray.sample(sampler, INF, lightSample.value);
        
        // volume scattering
        if (media && (raySample.value < rayLength)) {

            MediumInteraction mediumInteraction(ray.origin + ray.direction*raySample.value, scene.medium);
            mediumInteraction.pdf = raySample.pdf;
            vec3 directIllumination = getDirectIllumination(mediumInteraction, lightSample);
            colour +=throughput*(scene.medium->emission*raySample.value + directIllumination*scene.medium->scattering)*scene.medium->transmittance(raySample.value)/(raySample.pdf);
            return colour;
            
        }
        // surface scattering
        else if (interaction.hit) {

            throughput = throughput*scene.medium->transmittance(rayLength);

            // russian roulette
            double rr = interaction.material->albedo.value().luminance();
            if (interaction.emission != vec3() || throughput == vec3() || sampler.getRandomDouble() > rr) {
                return colour + throughput*interaction.emission + throughput*getDirectIllumination(interaction);
            }
            throughput /= rr;

            // sample surface brdf
            interaction.sample(sampler);

            // get the sampled ray
            ray = interaction.getIncoming();

            // next-event estimation (direct light sampling), don't do this if the bxdf is a delta function
            if (interaction.pdf != 1 && direct_light_sampling) {
                
                Interaction nextInteraction = scene.intersect(ray);

                // use multiple importance sampling if the next interaction happens to be on a light
                if (nextInteraction.hit && nextInteraction.emission != vec3()) { 
                    colour += throughput*getDirectIllumination(interaction, nextInteraction);
                    return colour;
                } else {
                    colour += throughput*getDirectIllumination(interaction);
                    throughput = throughput * interaction.brdf / interaction.pdf;
                    interaction = nextInteraction;
                }
               
            } 
            else {
                throughput = throughput * interaction.brdf / interaction.pdf;
                interaction = scene.intersect(ray);
            }

            
        } else {
            return colour;
        }
    }

    return colour;
}
