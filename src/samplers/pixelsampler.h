#pragma once

#include <vector>
#include "../core/smath.h"
#include "../core/sample.h"
#include "../core/sampler.h"
#include <random>

class PixelSampler {
public:
    PixelSampler() {};
    PixelSampler(int w, int h, int n, int d);
    Sample2D getStratifiedSample();
    Sample3D getRandomHemisphereSample();
    double getRandomDouble();
    std::mt19937 gen;

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