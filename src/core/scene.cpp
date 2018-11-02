#include "scene.h"
#include <algorithm>

Scene::Scene(std::vector<Shape*> geometry) {
    auto sorted(geometry);
    sort(sorted.begin(), sorted.end(), [](const Shape* a, const Shape* b){return a->p0.x > b->p0.x;});

}