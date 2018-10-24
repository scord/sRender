#include "diffuse.h"

Vector3 circleToHemisphere(double u1, double u2) {
	const double r = std::sqrt(u1);
	const double theta = 2 * 3.14159265 * u2;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);

    double height = std::sqrt(1-r*r);
    
	return Vector3(x, y, height);
}

DiffuseBrdf::DiffuseBrdf(){
    pdf = 0.3;
}

Ray DiffuseBrdf::Sample(Ray ray, Vector3 p, Vector3 n) {
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1);
    Vector3 test = circleToHemisphere(dis(gen), dis(gen));

    Ray newRay(p, test);

    newRay.rotateToVector(n);

    return newRay;
}