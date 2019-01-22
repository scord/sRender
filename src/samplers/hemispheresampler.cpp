
#include "hemispheresampler.h"
#include <random>


HemisphereSampler::HemisphereSampler() {

}

Vector3 quadToHemisphere(double u1, double u2) {
	const double r = std::sqrt(u1);
	const double theta = TAU * u2;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);

    double height = std::sqrt(1-r*r);
    
	return Vector3(x, y, height);
}


Sample3D HemisphereSampler::getSample(double u1, double u2) {
	Vector3 position = quadToHemisphere(u1, u2);
    return Sample3D(position, position.z*IPI);
}



