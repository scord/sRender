#include "pixelsampler.h"
#include <random>
#include <algorithm> 




PixelSampler::PixelSampler(int w, int h, int n, int d) {
    double grid_size = sqrt(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0,1);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            // stratified sampling
            for (int i = 0; i < grid_size; i++) {
                for (int j = 0; j < grid_size; j++) {
                    stratifiedSamples.push_back(Sample2D(Vector2(i/grid_size + dis(gen)/grid_size, j/grid_size + dis(gen)/grid_size), 1));
                    for (int k = 0; k < d; k++) {
                        randomSamples.push_back(Sample2D(Vector2(dis(gen), dis(gen)), 1));
                        randomDoubles.push_back(dis(gen));
                    }
                }
            }
        }
    }

    stratifiedSampleNumber = 0;
    randomSampleNumber = 0;
    randomDoubleNumber = 0;
}

Vector3 PixelSampler::quadToHemisphere(double u1, double u2) {
	const double r = std::sqrt(u1);
	const double theta = TAU * u2;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);

    double height = std::sqrt(1-r*r);
    
	return Vector3(x, y, height);
}


Sample2D PixelSampler::getStratifiedSample() {
    return stratifiedSamples[stratifiedSampleNumber++];
}

Sample3D PixelSampler::getRandomHemisphereSample() {
    Vector2 sample = randomSamples[randomSampleNumber++].value;
    Vector3 hemisphereSample = quadToHemisphere(sample.x, sample.y);
    return Sample3D(hemisphereSample, hemisphereSample.z/TAU);
}

double PixelSampler::getRandomDouble() {
    return randomDoubles[randomDoubleNumber++];
}