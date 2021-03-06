#include "scene.h"
#include "medium.h"
#include <algorithm>
#include <unordered_set>

Scene::Scene() {
    fill(new Medium());
}

void Scene::add(Object* object) {
    object->isVisible = true;
    objects.push_back(object);

    for (auto g : object->geometry) {
        if (g->isLight) {
            lightGeometry.push_back(g);
        }
    }
}

void Scene::fill(Medium* medium) {
    this->medium = medium;
}

void Scene::addLight(Object* object) {
    object->isVisible = false;
    objects.push_back(object);
    lights.push_back(object);
}

Object* Scene::getLight(Sampler& sampler) {
    double r = sampler.getRandomDouble();
    if (r < 0.5)
        return lights[0];
    else
        return lights[1];
}


Object* Scene::sampleLight(Sampler& sampler) {
    double totalArea = 0;
    double n_lights = lights.size();
    if (n_lights > 0) {
        int r = int(n_lights*sampler.getRandomDouble());
        return lights[r];
    } else {
        return nullptr;
    }
}


Interaction* Scene::intersectVisible(Ray ray) {

    double minT = 99999;
    bool intersectionFound = false;
    
    Object* intersectedObject;
    Shape* intersectedGeo;
 
    for (auto object : objects) {

        if (!object->isVisible)
            continue;
 
        Shape* geo;
        double t = object->intersect(ray, geo);

        if (t < minT && t > 0.000001) {
            minT = t;
    
            intersectedGeo = geo;
            intersectionFound = true;
            intersectedObject = object;
        }


    }

    if (intersectionFound) {
        vec3 intersection = ray.origin+ray.direction*minT;

        return new Interaction(vec3()-ray.direction,
                            intersection,
                            intersectedGeo->defaultUVMapping()(intersection),
                            intersectedGeo->normal(intersection),
                            intersectedObject->material,
                            true);
    } else {
        return nullptr;
    }
}



Interaction Scene::intersect(Ray ray) const {

    double minT = 99999;
    bool intersectionFound = false;
    
    Object* intersectedObject;
    Shape* intersectedGeo;
 
    for (auto object : objects) {
 
        Shape* geo;
        double t = object->intersect(ray, geo);

        if (t < minT && t > 0.000001) {
            minT = t;
    
            intersectedGeo = geo;
            intersectionFound = true;
            intersectedObject = object;
        }


    }

    if (intersectionFound) {
        vec3 intersection = ray.origin+ray.direction*minT;

        return Interaction(vec3()-ray.direction,
                            intersection,
                            intersectedGeo->defaultUVMapping()(intersection),
                            intersectedGeo->normal(intersection),
                            intersectedObject->material,
                            true);
    } else {
        return Interaction(vec3()-ray.direction, ray.origin+ray.direction*1000000.0);
    }
}

Object::Object(std::vector<Shape*> geometry, vec3 pos, double scale, Material* material) : geometry(geometry), transform(Transform(pos, vec3(scale, scale, scale), vec3())), aabb(calculateBoundingBox()), material(material) {
    for (int i = 0; i < geometry.size(); i++) {
        geometry[i]->transform(pos, vec3(scale, scale, scale));
    }    
}

KDTreeObject::KDTreeObject(std::vector<Shape*> geometry, vec3 pos, double scale, Material* material) : 
    Object(geometry, pos, scale, material) { 
    root = KDTreeNode(calculateBoundingBox(), this->geometry, 0);
}

Ray Object::objectSpaceRay(Ray ray) {
    vec3 p0 = ray.origin;
    vec3 p1 = ray.origin+ray.direction;

    vec3 tp0 = transform.applyInverse(p0);
    return Ray(tp0, (transform.applyInverse(p1) - tp0).norm());

}

BoundingBox Object::calculateBoundingBox() {
    vec3 min(0,0,0);
    vec3 max(0,0,0);
    
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
double Object::intersect(Ray ray, Shape*& intersectedGeometry) {
    


    double minT = 99999;

    bool intersectionFound = false;
    for (auto shape : geometry) {
        double t = shape->Intersect(ray);
        if (t < minT && t > 0) {
            minT = t;
            intersectedGeometry = (shape);
            intersectionFound = true;
        }
    }
   
    return minT;
   
}

double KDTreeObject::intersect(Ray ray, Shape*& intersectedGeometry) {


    double t = root.intersect(ray, intersectedGeometry);

    return t;
  
}