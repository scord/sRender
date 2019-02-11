#pragma once

#include <vector>
#include "../core/smath.h"
#include "../core/sample.h"

#include <random>

class Sampler {
public:
    Sampler() {};
    Sampler(int n, int spp, int d);
    Sample2D getStratifiedSample();
    Sample2D getSample2D();

    double getRandomDouble();
    std::mt19937 gen;
    int gridSize;
    int i;
    Vector3 quadToHemisphere(double u1, double u2);
    int stratifiedSampleNumber;
    int randomSampleNumber;
    int randomDoubleNumber;
    void generateTestImage(const int width, const int height);
protected:
    std::vector<Sample2D> stratifiedSamples;
    std::vector<Sample2D> randomSamples;
    std::vector<double> randomDoubles;
    std::uniform_real_distribution<double> dis;
};