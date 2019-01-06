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
    Vector3 getRadiance(Ray ray, int depth, Scene scene);
};