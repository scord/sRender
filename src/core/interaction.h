#pragma once

#include <vector>
#include "geometry.h"
#include "smath.h"


#include "material.h"


class Interaction {
public:
    Interaction();
    Interaction(Vector3 direction, Vector3 pos, Vector3 n, Material* material, bool backward);
    Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector3 n, Material* material, bool backward);

    Ray getOutgoing();
    Ray getIncoming();

    double cost;
    Sample3D sample(Sampler* sampler);
    Sample3D sample(Sampler* sampler, Vector3& reflectance);
    double getPdf();
    Vector3 getBrdf();

    Vector3 getReflectance();
    Vector3 position;
    Vector3 normal;
    Vector3 odir;
    Vector3 idir;

    Material* material;

    bool backward;

    Sampler sampler;
};