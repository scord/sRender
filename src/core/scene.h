#pragma once

#include <vector>
#include "geometry.h"
#include "kdtree.h"
#include <memory>

class Object {
public:
    Transform transform;
    Ray objectSpaceRay(Ray ray);
    std::vector<Shape*> geometry;
    Object() {};
    Object(std::vector<Shape*> geometry, Vector3 pos, float scale);
    BoundingBox aabb;
    BoundingBox calculateBoundingBox();
    virtual double intersect(Ray ray, Shape* &intersectedGeometry);
};

class KDTreeObject : public Object {
public:
    KDTreeObject() {};
    KDTreeObject(std::vector<Shape*> geometry, Vector3 pos, float scale);
    virtual double intersect(Ray ray, Shape* &intersectedGeometry);
    KDTreeNode root;
};

class Scene {
public:
    std::vector<Object*> objects;
    std::vector<Shape*> lightGeometry;

    Scene();
    void add(Object* object);
    bool intersect(Ray ray, Vector3 &intersection, Shape*  &intersectedGeometry);
    Ray sampleLight();
};

