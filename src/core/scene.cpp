#include "scene.h"
#include <algorithm>
#include <unordered_set>

Scene::Scene() {}

void Scene::add(Object* object) {
    objects.push_back(object);
    for (auto g : object->geometry) {
        if (g->isLight) {
            lightGeometry.push_back(g);
        }
    }
}

bool Scene::intersect(Ray ray, Vector3& intersection, Shape* &intersectedGeometry) {

    float minT = 99999;
    bool intersectionFound = false;
    
 
    for (auto object : objects) {
 
        Shape* intersectedGeo;
        double t = object->intersect(ray, intersectedGeo);

        if (t < minT && t > 0) {
            minT = t;
    
            intersectedGeometry = (intersectedGeo);
            intersectionFound = true;
        }


    }

 
    intersection = ray.origin+ray.direction*minT;

    return intersectionFound;
}

Object::Object(std::vector<Shape*> geometry, Vector3 pos, float scale) : geometry(geometry), transform(Transform(pos, Vector3(scale, scale, scale), Vector3())), aabb(calculateBoundingBox()) {
    for (int i = 0; i < geometry.size(); i++) {
        geometry[i]->transform(pos, Vector3(scale, scale, scale));
    }
} 

KDTreeObject::KDTreeObject(std::vector<Shape*> geometry, Vector3 pos, float scale) : 
    Object(geometry, pos, scale) { 
    root = KDTreeNode(calculateBoundingBox(), this->geometry, 0);
}

Ray Object::objectSpaceRay(Ray ray) {
    Vector3 p0 = ray.origin;
    Vector3 p1 = ray.origin+ray.direction;

    Vector3 tp0 = transform.applyInverse(p0);
    return Ray(tp0, (transform.applyInverse(p1) - tp0).norm());

}

BoundingBox Object::calculateBoundingBox() {
    Vector3 min(0,0,0);
    Vector3 max(0,0,0);
    
    for (auto shape : geometry) {
        if (shape->aabb.min.x < min.x) {
            min.x = shape->aabb.min.x;
        }
        if (shape->aabb.min.y < min.y) {
            min.y = shape->aabb.min.y;
        }
        if (shape->aabb.min.z < min.z) {
            min.z = shape->aabb.min.z;
        }

        if (shape->aabb.max.x > max.x) {
            max.x = shape->aabb.max.x;
        }
        if (shape->aabb.max.y > max.y) {
            max.y = shape->aabb.max.y;
        }
        if (shape->aabb.max.z > max.z) {
            max.z = shape->aabb.max.z;
        }
    }

    return BoundingBox(min,max);
}
double Object::intersect(Ray ray, Shape* &intersectedGeometry) {
    
    Vector3 minIntersectionPoint;

    double minT = 99999;

    bool intersectionFound = false;
    for (auto shape : geometry) {
        double t = shape->Intersect(ray, minIntersectionPoint);
        if (t < minT && t > 0) {
            minT = t;
            intersectedGeometry = (shape);
            intersectionFound = true;
        }
    }
   
    return minT;
   
}

double KDTreeObject::intersect(Ray ray, Shape* &intersectedGeometry) {

    std::vector<Shape*> foundGeometry = root.findGeometry(ray);
    
    Vector3 minIntersectionPoint;

    double minT = 99999;

    bool intersectionFound = false;
    for (auto& shape : foundGeometry) {


    
        double t = shape->Intersect(ray, minIntersectionPoint);

        if (t < minT && t > 0) {
            minT = t;
            intersectedGeometry = (shape);
            intersectionFound = true;
        }
          
        
    }

    return minT;
  
}