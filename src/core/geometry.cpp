#include "geometry.h"
#include "../core/smath.h"


Shape::Shape(Vector3 p0, Vector3 colour, Brdf* brdf) : p0(p0), colour(colour), isLight(false), objectToWorld(Transform(p0, Vector3(), Vector3())) {
    this->brdf = brdf;
}

Plane::Plane(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf) : Shape(p0, colour, brdf), n(n) {
    if (std::abs(n.x) > std::abs(n.y)) {
		double invLen = 1.f / sqrtf(n.x * n.x + n.z * n.z);
		up = Vector3(-n.z * invLen, 0.0f, n.x * invLen);
    } else {
		double invLen = 1.0f / sqrtf(n.y * n.y + n.z * n.z);
		up = Vector3(0.0f, n.z * invLen, -n.y * invLen);
    }
    left = n.cross(up);
}

Vector3 Plane::normal(Vector3 p) {
    return n;
}

double Plane::Intersect(Ray ray, Vector3& intersection) {
    double np = ray.direction.dot(n);
    double t = (p0.dot(n)-ray.origin.dot(n))/np;
    intersection = ray.origin + ray.direction*t;
    return t;
}

Disc::Disc(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size) : Plane(p0, n, colour, brdf), size(size) {}

double Disc::Intersect(Ray ray, Vector3& intersection) {
    double t = Plane::Intersect(ray, intersection);
    if ((intersection - p0).length() < size.length()) {
        return t;
    } else {
        return -1;
    }
}

Quad::Quad(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size, Vector3 up) : Plane(p0, n, colour, brdf), size(size) {
    this->up = up;
    left = up.cross(n);
}

double Quad::Intersect(Ray ray, Vector3& intersection) {
    double t = Plane::Intersect(ray, intersection);
    Vector3 d = intersection - p0;
    Vector2 dist = Vector2(d.dot(left), d.dot(up));
    if (dist.x <= size.x && dist.x > -size.x && dist.y <= size.y && dist.y > -size.y) {
        return t; 
    } else {
        return -1;
    }
}

Sphere::Sphere(Vector3 p0, Vector3 colour, Brdf* brdf, double r) : Shape(p0, colour, brdf), radius(r) {}

double Sphere::Intersect(Ray ray, Vector3& intersection) {
    Vector3 dir2 = ray.origin-p0;
    double b = (dir2*2).dot(ray.direction);
    double c = dir2.dot(dir2) - (radius*radius);
    double d = b*b - 4*c;
    if (d<0) {
        return -1;
    }
    d = std::sqrt(d);
    double s1 = -b + d;
    double s2 = -b - d;
    
    double t = (s2>0) ? s2/2 : ((s1>0) ? s1/2 : 0);
    intersection = ray.origin + ray.direction*t;
    return t;

}

Vector3 Sphere::normal(Vector3 p) {
    return (p - p0).norm();
}



