#pragma once

#include <vector>
#include "geometry.h"
#include "smath.h"
#include "scene.h"
#include "../samplers/sampler.h"

class Integrator {
public:
    Integrator();
    Integrator(int maxDepth);
    int maxDepth;
    Vector3 getRadiance(Ray ray, int depth, Scene* scene, Sampler* sampler);
    Vector3 getDirectIlluminationMIS(Interaction* interactionP, Interaction* nextInteractionP, Scene* scene, Disc* light, Sampler* sampler);
    Vector3 getDirectIllumination(Interaction* interactionP, Scene* scene, Disc* light, Sampler* sampler);
    Vector3 getIllumination(Interaction* interaction, Interaction* nextInteractionP, Scene* scene, Sampler* sampler);
    double balanceHeuristic(double pdf1, double pdf2);
    double powerHeuristicDividedByPdf(double pdf, double pdf2);
};