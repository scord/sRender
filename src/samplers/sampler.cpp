#include "sampler.h"
#include <random>
#include <algorithm> 

Sampler::Sampler(int n, int spp, int d) {
 //   double grid_size = sqrt(spp);
    std::random_device rd;
    std::mt19937 gen(rd());
    dis = std::uniform_real_distribution<double>(0,1);
    this->gen = gen;
  /*  for (int x = 0; x < n; x++) {
            for (int i = 0; i < grid_size; i++) {
                for (int j = 0; j < grid_size; j++) {
                    stratifiedSamples.push_back(Sample2D(Vector2(i/grid_size + dis(gen)/grid_size, j/grid_size + dis(gen)/grid_size), 1));


                    for (int k = 0; k < d; k++) {
                        randomSamples.push_back(Sample2D(Vector2(dis(gen), dis(gen)), 1));
                        randomDoubles.push_back((float)dis(gen));
                       // randomDoubles.push_back((float)dis(gen));
                       // randomDoubles.push_back((float)dis(gen));
                       // randomDoubles.push_back((float)dis(gen));

                        //randomDoubles.push_back(dis(gen));
                    }
                    
                }
            }
        
    }*/


   // stratifiedSampleNumber = stratifiedSamples.size() - 1;
   // randomSampleNumber = randomSamples.size() - 1;
   // randomDoubleNumber = randomDoubles.size() - 1;
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
    return Sample2D(Vector2(dis(gen), dis(gen)), 1);
}

Sample2D Sampler::getSample2D() {
    return Sample2D(Vector2(dis(gen), dis(gen)), 1);
}

double Sampler::getRandomDouble() {
    return dis(gen);
}