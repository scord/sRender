#pragma once

#include "../core/smath.h"
#include <vector>
#include "sample.h"
#include <functional>
#include "../samplers/sampler.h"


struct Ray {
    vec3 origin;
    vec3 direction;
    Ray(vec3 origin, vec3 direction);
    Sample<double> sample(Sampler& sampler, double max, double sigma);
    Sample<double> sample(Sampler& sampler, double max);
    Sample<double> sample(Sampler& sampler, double max, vec3 p);
    void rotateToVector(vec3 v);
};


class BoundingBox {
public:
    vec3 min;
    vec3 max;
    BoundingBox() {};
    BoundingBox(vec3 min, vec3 max);
    bool contains(vec3 p);
    bool intersects(Ray ray);
    bool overlaps(BoundingBox box);
    void transform(vec3 position, vec3 scale);
    double intersectPlane(vec3 p, vec3 n, Ray ray);

    std::vector<vec3> intersect(Ray ray);
};


class Shape {
public:
    Shape(vec3 p0);
    Transform objectToWorld;
    vec3 p0;
    
    vec3 up;
    vec3 left;
    virtual Sample3D sample(double u1, double u2) {return Sample3D(vec3(),1.0);};
    bool isLight;
    BoundingBox aabb;
    virtual BoundingBox calculateBoundingBox() = 0;
    virtual vec3 samplePoint() = 0;
    virtual vec3 normal(vec3 p) = 0;
    virtual double Intersect(Ray ray) = 0;
    virtual void transform(vec3 position, vec3 scale) = 0;
    virtual std::function<vec2(vec3)> defaultUVMapping();
};



class Triangle : public Shape {
public:
    Triangle(vec3 v0, vec3 v1, vec3 v2);
    vec3 n;
    vec3 v1, v2;
    
    virtual vec3 normal(vec3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual vec3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(vec3 position, vec3 scale);
};


    
class Plane : public Shape {
public:
    vec3 n;
    Plane(vec3 p0, vec3 n);
    virtual vec3 normal(vec3 p) ;
    
    virtual BoundingBox calculateBoundingBox();
    virtual vec3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(vec3 position, vec3 scale);
};

class Disc : public Plane {
public:
    Disc(vec3 p0, vec3 n, vec2 size);
    vec2 size;
    double area;
    virtual BoundingBox calculateBoundingBox();
    virtual vec3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(vec3 position, vec3 scale);
    double calculateArea();
    Sample3D sample(double u1, double u2);
    Matrix3 rotationMatrix;
    virtual std::function<vec2(vec3)> defaultUVMapping();

};

class Quad : public Plane {
public:
    Quad(vec3 p0, vec3 n, vec2 size, vec3 up);
    vec2 size;
    virtual BoundingBox calculateBoundingBox();
    virtual vec3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(vec3 position, vec3 scale);
    virtual std::function<vec2(vec3)> defaultUVMapping();
};

class Sphere : public Shape {
public:
    Sphere(vec3 p0, double radius);
    double radius;
    virtual vec3 normal(vec3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual vec3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(vec3 position, vec3 scale);
};

class Hemisphere : public Shape {
public:
    Hemisphere(vec3 p0, double radius);
    double radius;
    virtual Sample3D map(Sample2D sample);
    virtual vec3 normal(vec3 p);
    virtual BoundingBox calculateBoundingBox();
    virtual vec3 samplePoint();
    virtual double Intersect(Ray ray);
    virtual void transform(vec3 position, vec3 scale);
};

vec2 planarUVMapping(vec3 pos, vec3 p0, vec3 left, vec3 up, vec2 size);