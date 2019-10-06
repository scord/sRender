#pragma once

#include <vector>
#include "geometry.h"
#include "smath.h"
#include "medium.h"


#include "material.h"


class Interaction {
public:
    Interaction();
    Interaction(vec3 direction, vec3 pos);
    Interaction(vec3 direction, vec3 pos, vec2 uv, vec3 n, Material* material, bool backward);

    Interaction(vec3 odir, vec3 idir, vec3 pos, vec2 uv, vec3 n, Material* material, bool backward, double pdf);

    Ray getOutgoing();
    Ray getIncoming();

    bool hit;

    double cost;
    void sample(Sampler& sampler);
    void forceSample(vec3 idir, double pdf);
    double getPdf();
    vec3 getBrdf();
    vec3 emission;

    vec3 position;
    vec2 uv;
    vec3 normal;
    vec3 odir;
    vec3 idir;
    vec3 brdf;
    double pdf;

    Material* material;

    bool backward;

    Sampler sampler;
};

class MediumInteraction {
public:
    MediumInteraction();
    MediumInteraction(vec3 pos, Medium* medium);

    vec3 position;
    double pdf;
    Medium* medium;

};