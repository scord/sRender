#pragma once

#include <vector>
#include "geometry.h"
#include "smath.h"
#include "scene.h"
#include "../samplers/sampler.h"

class Integrator {
public:
    Integrator();
    Integrator(int maxDepth, const Scene& scene, Sampler& sampler);
    vec3 getRadiance(Ray ray, int depth);
private:
    int maxDepth;
    Scene scene;
    bool media;
    bool direct_light_sampling;
    Sampler sampler;
    vec3 radiance(const Interaction& interaction, const Interaction& nextInteraction);
    vec3 getDirectIllumination(const Interaction& interaction, const Interaction& nextInteraction);
    vec3 getDirectIllumination(const Interaction& interaction);
    vec3 getDirectIllumination(const MediumInteraction& interaction, Sample3D lSample);
    Sample<Object*> chooseLight();
    Sample3D sampleLights(const vec3& pos);
    Sample3D sampleLights();
    vec3 getIllumination(Interaction* interaction, Interaction* nextInteractionP);
    double balanceHeuristic(double pdf1, double pdf2);
    double powerHeuristicDividedByPdf(double pdf, double pdf2);
};