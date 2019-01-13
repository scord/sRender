#include "../core/sampler.h"
#include "discsampler.h"
#include <random>



Vector2 quadToDisc(double u1, double u2) {
	const double r = std::sqrt(u1);
	const double theta = TAU * u2;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);
    
	return Vector2(x, y);
}




Sample2D DiscSampler::getSample() {
    Sample2D sample = Sampler::getSample();
    return Sample2D(sample.value, IPI);
}



