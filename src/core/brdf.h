#pragma once

#include "../core/smath.h"
#include <random>

class Brdf {
public:
    Brdf();
    std::random_device rd;
    double pdfDividedByCos;
    virtual Ray Sample(Ray ray, Vector3 p, Vector3 n) = 0;
};