#pragma once

#include <vector>
#include "geometry.h"
#include "smath.h"


#include "material.h"


class Interaction {
public:
    Interaction();
    Interaction(Vector3 direction, Vector3 pos, Vector2 uv, Vector3 n, Material* material, bool backward);

    Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector2 uv, Vector3 n, Material* material, bool backward, double pdf);

    Ray getOutgoing();
    Ray getIncoming();

    double cost;
    void sample(Sampler* sampler);
    void forceSample(Vector3 idir, double pdf);
    double getPdf();
    Vector3 getBrdf();
    Vector3 emission;

    Vector3 position;
    Vector2 uv;
    Vector3 normal;
    Vector3 odir;
    Vector3 idir;
    Vector3 brdf;
    double pdf;

    Material* material;

    bool backward;

    Sampler sampler;
};