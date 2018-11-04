#include <vector>
#include "geometry.h"
#include "smath.h"
#include "scene.h"

class Integrator {
public:
    Integrator();
    int maxDepth;
    Vector3 getRadiance(Ray ray, int depth, Scene scene);
};