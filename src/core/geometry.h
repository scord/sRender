#pragma once

#include "../core/smath.h"
#include <vector>
#include "sample.h"
#include <functional>

class BoundingBox {
public:
    Vector3 min;
    Vector3 max;
    BoundingBox() {};
    BoundingBox(Vector3 min, Vector3 max);
    bool contains(Vector3 p);
    bool intersects(Ray ray);
    bool overlaps(BoundingBox box);
    void transform(Vector3 position, Vector3 scale);
    double intersectPlane(Vector3 p, Vector3 n, Ray ray);

    std::vector<Vector3> intersect(Ray ray);
};


class Shape {
public:
    Shape(Vector3 p0);
    Transform objectToWorld;
    Vector3 p0;
    
    Vector3 up;
    Vector3 left;
    
    bool isLight;
    BoundingBox aabb;
    virtual BoundingBox calculateBoundingBox() = 0;
    virtual Vector3 samplePoint() = 0;
    virtual Vector3 normal(Vector3 p) = 0;
    virtual double Intersect(Ray ray) = 0;
    virtual void transform(Vector3 position, Vector3 scale) = 0;
    virtual std::function<Vector2(Vector3)> defaultUVMapping();
};



class Triangle : public Shape {
public:
    Triangle(Vector3 v0, Vector3 v1, Vector3 v2);
    Vector3 n;
    Vector3 v1, v2;
    
    virtual Vector3 normal(Vector3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(Vector3 position, Vector3 scale);
};


    
class Plane : public Shape {
public:
    Vector3 n;
    Plane(Vector3 p0, Vector3 n);
    virtual Vector3 normal(Vector3 p) ;
    
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(Vector3 position, Vector3 scale);
};

class Disc : public Plane {
public:
    Disc(Vector3 p0, Vector3 n, Vector2 size);
    Vector2 size;
    double area;
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(Vector3 position, Vector3 scale);
    double calculateArea();
    Sample3D sample(double u1, double u2);
    Matrix3 rotationMatrix;
    virtual std::function<Vector2(Vector3)> defaultUVMapping();

};

class Quad : public Plane {
public:
    Quad(Vector3 p0, Vector3 n, Vector2 size, Vector3 up);
    Vector2 size;
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(Vector3 position, Vector3 scale);
    virtual std::function<Vector2(Vector3)> defaultUVMapping();
};

class Sphere : public Shape {
public:
    Sphere(Vector3 p0, double radius);
    double radius;
    virtual Vector3 normal(Vector3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(Vector3 position, Vector3 scale);
};

class Hemisphere : public Shape {
public:
    Hemisphere(Vector3 p0, double radius);
    double radius;
    virtual Sample3D map(Sample2D sample);
    virtual Vector3 normal(Vector3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual Vector3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(Vector3 position, Vector3 scale);
};

Vector2 planarUVMapping(Vector3 pos, Vector3 p0, Vector3 left, Vector3 up, Vector2 size);