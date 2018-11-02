#include <vector>
#include "geometry.h"

class Scene {
    std::vector<Shape*> geometry;

    Scene(std::vector<Shape*> geometry);
    void add(Shape* shape);
    double intersect(Ray ray, Vector3 intersection);
};