#include "geometry.h"
#include "../core/smath.h"
#include <cmath>


Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 colour, Brdf* brdf) : Shape(v0, colour, brdf), v1(v1), v2(v2) {
    n = (v1 - v0).cross(v2 - v0).norm();
    this->brdf = brdf;
    aabb = calculateBoundingBox();
}

Vector3 Triangle::samplePoint() {

}
Vector3 Disc::samplePoint() {
    
}
Vector3 Quad::samplePoint() {
    
}
Vector3 Sphere::samplePoint() {
    
}
Vector3 Plane::samplePoint() {
    
}


Vector3 Triangle::normal(Vector3 p) {
    return n;
}


double Triangle::Intersect(Ray ray, Vector3& intersection2) {

    Vector3 tv0 = p0;
    Vector3 tv1 = v1;
    Vector3 tv2 = v2;

    double np = ray.direction.dot(n);
    double t = (tv0.dot(n)-ray.origin.dot(n))/np;
    Vector3 intersection = ray.origin + ray.direction*t;
    
    Vector3 edge0 = tv1 - tv0;
    Vector3 vp0 = intersection - tv0;
    if (n.dot(edge0.cross(vp0)) < 0) return -1;

    Vector3 edge1 = tv2 - tv1;
    Vector3 vp1 = intersection - tv1;
    if (n.dot(edge1.cross(vp1)) < 0) return -1;

    Vector3 edge2 = tv0 - tv2;
    Vector3 vp2 = intersection - tv2;
    if (n.dot(edge2.cross(vp2)) < 0) return -1;

    return t;
}

void Triangle::transform(Vector3 position, Vector3 scale) {
    p0.x *= scale.x;
    p0.y *= scale.y;
    p0.z *= scale.z;
    p0.x += position.x;
    p0.y += position.y;
    p0.z += position.z;

    v1.x *= scale.x;
    v1.y *= scale.y;
    v1.z *= scale.z;
    v1.x += position.x;
    v1.y += position.y;
    v1.z += position.z;

    v2.x *= scale.x;
    v2.y *= scale.y;
    v2.z *= scale.z;
    v2.x += position.x;
    v2.y += position.y;
    v2.z += position.z;

    aabb.transform(position, scale);
}

void BoundingBox::transform(Vector3 position, Vector3 scale) {
    min.x *= scale.x;
    min.y *= scale.y;
    min.z *= scale.z;
    min += position;

    max.x *= scale.x;
    max.y *= scale.y;
    max.z *= scale.z;
    max += position;
}
BoundingBox::BoundingBox(Vector3 min, Vector3 max) : min(min), max(max) {}

bool BoundingBox::overlaps(BoundingBox box) {
    if (contains(box.min) || contains(box.max)) {
        return true;
    } else {
        return false;
    }
}

bool BoundingBox::contains(Vector3 p) {
    if (p.x >= min.x && p.y >= min.y && p.z >= min.z && p.x <= max.x && p.y <= max.y && p.z <= max.z) {
        return true;
    } else {
        return false;
    }
}

bool BoundingBox::intersects(Ray ray) {

    double txmin, tymin, tzmin;
    double txmax, tymax, tzmax;

    if (ray.direction.x >= 0) {
        txmin = (min.x - ray.origin.x) / ray.direction.x;
        txmax = (max.x - ray.origin.x) / ray.direction.x;
    } else {
        txmax = (min.x - ray.origin.x) / ray.direction.x;
        txmin = (max.x - ray.origin.x) / ray.direction.x;
    }

    if (ray.direction.y >= 0) {
        tymin = (min.y - ray.origin.y) / ray.direction.y;
        tymax = (max.y - ray.origin.y) / ray.direction.y;
    } else {
        tymax = (min.y - ray.origin.y) / ray.direction.y;
        tymin = (max.y - ray.origin.y) / ray.direction.y;
    }

    if ((txmin > tymax) || (tymin > txmax))
        return false;
    if (tymin > txmin)
        txmin = tymin;
    if (tymax < txmax)
        txmax = tymax;

    if (ray.direction.z >= 0) {
        tzmin = (min.z - ray.origin.z) / ray.direction.z;
        tzmax = (max.z - ray.origin.z) / ray.direction.z;
    } else {
        tzmax = (min.z - ray.origin.z) / ray.direction.z;
        tzmin = (max.z - ray.origin.z) / ray.direction.z;
    }
    if ((txmin > tzmax) || (tzmin > txmax))
        return false;
    if (tzmin > txmin)
        txmin = tzmin;
    if (tzmax < txmax)
        txmax = tzmax;

    return true; 
}

BoundingBox Triangle::calculateBoundingBox() {
    double minX = std::min(p0.x, std::min(v1.x, v2.x));
    double minY = std::min(p0.y, std::min(v1.y, v2.y));
    double minZ = std::min(p0.z, std::min(v1.z, v2.z));

    double maxX = std::max(p0.x, std::max(v1.x, v2.x));
    double maxY = std::max(p0.y, std::max(v1.y, v2.y));
    double maxZ = std::max(p0.z, std::max(v1.z, v2.z));

    return BoundingBox(Vector3(minX,minY,minZ),
                       Vector3(maxX,maxY,maxZ));
}

Shape::Shape(Vector3 p0, Vector3 colour, Brdf* brdf) : p0(p0), colour(colour), isLight(false), objectToWorld(Transform(Vector3(0,0,0), Vector3(1,1,1), Vector3())), aabb(BoundingBox(Vector3(),Vector3())) {
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
    if (np > 0){
        return 0;
    }
    double t = (p0.dot(n)-ray.origin.dot(n))/np;
    intersection = ray.origin + ray.direction*t;
    return t;
}

BoundingBox Plane::calculateBoundingBox() {
    return BoundingBox(Vector3(), Vector3());
}

void Plane::transform(Vector3 position, Vector3 scale) {
    p0 += position;
}



Disc::Disc(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size) : Plane(p0, n, colour, brdf), size(size) {
    area = calculateArea();
    rotationMatrix = n.rotationMatrix();
}

double Disc::Intersect(Ray ray, Vector3& intersection) {
    double t = Plane::Intersect(ray, intersection);
    if ((intersection - p0).length() < size.length()) {
        return t;
    } else {
        return -1;
    }
}

BoundingBox Disc::calculateBoundingBox() {
    return BoundingBox(Vector3(), Vector3());
}

void Disc::transform(Vector3 position, Vector3 scale) {
    p0 += position;
    size.x *= scale.x;
    size.y *= scale.y;
    area = calculateArea();
}

double Disc::calculateArea() {
    return PI*size.x*size.x;
}

Sample3D Disc::sample(double u1, double u2) {
    const double r = size.x*std::sqrt(u1);
	const double theta = TAU * u2;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);
    const double z = 0;

    Vector3 pos = p0 + rotationMatrix*Vector3(x,y,z);

	return Sample3D(pos, 1/area);
}


Quad::Quad(Vector3 p0, Vector3 n, Vector3 colour, Brdf* brdf, Vector2 size, Vector3 up) : Plane(p0, n, colour, brdf), size(size) {
    this->up = up;
    left = up.cross(n);
}

BoundingBox Quad::calculateBoundingBox() {
    return BoundingBox(Vector3(), Vector3());
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

void Quad::transform(Vector3 position, Vector3 scale) {
    p0 += position;
    size.x *= scale.x;
    size.y *= scale.y;
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
    
    double t = (s2>0.0000001) ? s2/2 : ((s1>0.0000001) ? s1/2 : 0);
    intersection = ray.origin + ray.direction*t;
    return t;

}

void Sphere::transform(Vector3 position, Vector3 scale) {
    p0 += position;
    radius *= scale.x;
}

BoundingBox Sphere::calculateBoundingBox() {
    return BoundingBox(Vector3(), Vector3());
}

Vector3 Sphere::normal(Vector3 p) {
    return (p - p0).norm();
}



