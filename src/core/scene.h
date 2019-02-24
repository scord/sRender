#pragma once

#include <vector>
#include "geometry.h"
#include "kdtree.h"
#include <memory>
#include "material.h"
#include "interaction.h"

class Object {
public:
    Transform transform;
    Ray objectSpaceRay(Ray ray);
    std::vector<Shape*> geometry;
    Material* material;
    Object() {};
    Object(std::vector<Shape*> geometry, Vector3 pos, double scale, Material* material);
    BoundingBox aabb;
    BoundingBox calculateBoundingBox();
    virtual double intersect(Ray ray, Shape* &intersectedGeometry);
};

class KDTreeObject : public Object {
public:
    KDTreeObject() {};
    Material* material;
    KDTreeObject(std::vector<Shape*> geometry, Vector3 pos, double scale, Material* material);
    virtual double intersect(Ray ray, Shape* &intersectedGeometry);
    KDTreeNode root;
};

class Scene {
public:
    std::vector<Object*> objects;
    std::vector<Object*> lights;
    std::vector<Shape*> lightGeometry;

    Scene();
    void add(Object* object);
    void addLight(Object* object);
    Object* getLight(Sampler* sampler);
    Object* sampleLight(Sampler* sampler);
    Interaction* intersect(Ray ray);

    Ray sampleLight();
};

