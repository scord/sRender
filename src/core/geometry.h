#pragma once

#include "../core/smath.h"
#include <vector>
#include "brdf.h"


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
    virtual Vector3 normal(Vector3 p) = 0;
    virtual double Intersect(Ray ray, Vector3 &intersection) = 0;
};
    
class Plane : public Shape {
public:
    Vector3 n;
    Plane(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf);
    virtual Vector3 normal(Vector3 p) ;
    virtual double Intersect(Ray ray, Vector3 &intersection);
};

class Disc : public Plane {
public:
    Disc(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size);
    Vector2 size;
    virtual double Intersect(Ray ray, Vector3 &intersection);
};

class Quad : public Plane {
public:
    Quad(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size, Vector3 up);
    Vector2 size;
    virtual double Intersect(Ray ray, Vector3 &intersection);
};

class Sphere : public Shape {
public:
    Sphere(Vector3 p0, Vector3 colour, Brdf* brdf, double radius);
    double radius;
    virtual Vector3 normal(Vector3 p);
    virtual double Intersect(Ray ray, Vector3 &intersection);
};