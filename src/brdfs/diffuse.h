#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/brdf.h"

class DiffuseBrdf : public Brdf {
public:
    DiffuseBrdf();
    virtual Ray Sample(Ray ray, Vector3 p, Vector3 n);
};