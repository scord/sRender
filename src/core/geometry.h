#pragma once

#include "../core/smath.h"
#include <vector>
#include "brdf.h"

class BoundingBox {
public:
    Vector3 min;
    Vector3 max;
    BoundingBox(Vector3 min, Vector3 max);
    bool contains(Vector3 p);
    bool intersects(Ray ray);
    bool overlaps(BoundingBox box);
};

class Shape {
public:
    Shape(Vector3 p0, Vector3 colour, Brdf* brdf);
    Transform objectToWorld;
    Vector3 p0;
    Vector3 colour;
    Vector3 up;
    Vector3 left;
    Brdf* brdf;
    bool isLight;
    BoundingBox aabb;
    virtual BoundingBox calculateBoundingBox() = 0;
    virtual Vector3 samplePoint() = 0;
    virtual Vector3 normal(Vector3 p) = 0;
    virtual double Intersect(Ray ray, Vector3 &intersection) = 0;
};



class Triangle : public Shape {
public:
    Triangle(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 colour, Brdf* brdf);
    Vector3 n;
    Vector3 v1, v2;
    Vector3 colour;
    virtual Vector3 normal(Vector3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray, Vector3& intersection);
};


    
class Plane : public Shape {
public:
    Vector3 n;
    Plane(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf);
    virtual Vector3 normal(Vector3 p) ;
    
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray, Vector3 &intersection);
};

class Disc : public Plane {
public:
    Disc(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size);
    Vector2 size;
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray, Vector3 &intersection);
};

class Quad : public Plane {
public:
    Quad(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size, Vector3 up);
    Vector2 size;
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray, Vector3 &intersection);
};

class Sphere : public Shape {
public:
    Sphere(Vector3 p0, Vector3 colour, Brdf* brdf, double radius);
    double radius;
    virtual Vector3 normal(Vector3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray, Vector3 &intersection);
};
