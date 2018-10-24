#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"

class MirrorBrdf : public Brdf {
public:
    MirrorBrdf();
    virtual Ray Sample(Ray ray, Vector3 p, Vector3 n);
};