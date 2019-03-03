#include "sampler.h"
#include <random>
#include <algorithm> 

Sampler::Sampler(int n, int spp, int d) {
    std::random_device rd;
    std::mt19937 gen(rd());
    dis = std::uniform_real_distribution<double>(0,1);
    gridSize = std::sqrt(spp);
    gridSize2 = gridSize*gridSize;
    i = 0;
    this->gen = gen;
}

Vector3 Sampler::quadToHemisphere(double u1, double u2) {
	const double r = std::sqrt(u1);
	const double theta = TAU * u2;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);

    double height = std::sqrt(1-r*r);
    
	return Vector3(x, y, height);
}


Sample2D Sampler::getStratifiedSample() {
    
    i = i % (gridSize2);
    int x = int(i/gridSize);
    int  y = i % gridSize;
    i++;
    
    return Sample2D(Vector2(x/(double)gridSize + dis(gen)/(double)gridSize, y/(double)gridSize + dis(gen)/(double)gridSize), 1);
}

Sample2D Sampler::getSample2D() {
    if (recycledDouble == 0) {
        return Sample2D(Vector2(dis(gen), dis(gen)), 1);
    } else {
        double r = recycledDouble;
        recycledDouble = 0;
        return Sample2D(Vector2(r, dis(gen)), 1);
    }
}

double Sampler::getRandomDouble() {
    if (recycledDouble == 0) {
        return dis(gen);
    } else {
        double r = recycledDouble;
        recycledDouble = 0;
        return r;
    }
}