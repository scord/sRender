#include <vector>
#include "geometry.h"
#include "smath.h"

class Integrator {
public:
    Integrator();
    int maxDepth;
    Vector3 getRadiance(Ray ray, int depth, std::vector<Shape*> scene);
};