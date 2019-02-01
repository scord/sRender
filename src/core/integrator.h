#include <vector>
#include "geometry.h"
#include "smath.h"
#include "scene.h"
#include "../samplers/pixelsampler.h"

class Integrator {
public:
    Integrator();
    Integrator(PixelSampler& sampler, int maxDepth);
    PixelSampler sampler;
    int maxDepth;
    Vector3 getRadiance(Ray ray, int depth, Scene* scene, Disc* light);
    Vector3 getDirectIllumination(Vector3 pos, Shape* object, Scene* scene, Disc* light);
    double balanceHeuristic(double pdf1, double pdf2);
    double balanceHeuristicDividedByPdf(double pdf, double pdf2);
};