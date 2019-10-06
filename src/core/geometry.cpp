#include "geometry.h"
#include "../core/smath.h"
#include <cmath>
#include <iostream>
#include <cassert>


Sample<double> Ray::sample(Sampler& sampler, double max, double sigma) {
    double u = sampler.getRandomDouble();

    double minU = exp(-sigma*max);
    double a = u*(1.0 - minU) + minU;

    return Sample<double>(-log(a)/sigma, sigma*a / (1.0-minU));   
}

Sample<double> Ray::sample(Sampler& sampler, double max, vec3 p) {

    double u = sampler.getRandomDouble();
    double close = (p - origin).dot(direction);

    double dist = (origin + direction*close - p).length();

    double thetaA = atan((0.0 - close)/dist);
    double thetaB = atan((max - close)/dist);

    double phi = (1-u)*thetaA + u*thetaB;
    
    double t = dist * tan(phi);

    return Sample<double>(
        close + t,
        dist / ((thetaB-thetaA)*(dist*dist+t*t))
    );   
}

Sample<double> Ray::sample(Sampler& sampler, double max) {
    double u = sampler.getRandomDouble();
    return Sample<double>(u*max, 1.0/max);   
}

Ray::Ray(vec3 origin, vec3 direction):origin(origin), direction(direction) {}
//0,0,-1

void Ray::rotateToVector(vec3 v) {
    vec3 up;
    if (std::abs(v.x) > std::abs(v.y)) {
		double invLen = 1.f / sqrt(v.x * v.x + v.z * v.z);
		up = vec3(-v.z * invLen, 0.0f, v.x * invLen);
    } else {
		double invLen = 1.0f / sqrt(v.y * v.y + v.z * v.z);
		up = vec3(0.0f, v.z * invLen, -v.y * invLen);
    }
    vec3 left = v.cross(up);

    vec3 rotatedDir;
	rotatedDir.x = vec3(up.x, 
                           left.x, 
                           v.x).dot(direction);

	rotatedDir.y = vec3(up.y, 
                           left.y, 
                           v.y).dot(direction);

	rotatedDir.z = vec3(up.z, 
                           left.z, 
                           v.z).dot(direction);
	
    direction = rotatedDir;
}
Triangle::Triangle(vec3 v0, vec3 v1, vec3 v2) : Shape(v0), v1(v1), v2(v2) {
    n = ((v1 - v0).norm()).cross((v2 - v0).norm()).norm();
    aabb = calculateBoundingBox();
}

vec3 Triangle::samplePoint() {
    return vec3();
}
vec3 Disc::samplePoint() {
    return vec3();   
}
vec3 Quad::samplePoint() {
    return vec3();    
}
vec3 Sphere::samplePoint() {
    return vec3();    
}
vec3 Hemisphere::samplePoint() {
    return vec3();    
}
vec3 Plane::samplePoint() {
    return vec3();    
}



vec3 Triangle::normal(vec3 p) {
    return n;
}


double Triangle::Intersect(Ray ray) {


    double np = ray.direction.dot(n);
    double t = (p0.dot(n)-ray.origin.dot(n))/np;
    vec3 intersection = ray.origin + ray.direction*t;
    
    if (n.dot((v1 - p0).cross(intersection - p0)) < 0) return -1;
    if (n.dot((v2 - v1).cross(intersection - v1)) < 0) return -1;
    if (n.dot((p0 - v2).cross(intersection - v2)) < 0) return -1;

    return t;
}

void Triangle::transform(vec3 position, vec3 scale) {
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

void BoundingBox::transform(vec3 position, vec3 scale) {
    min.x *= scale.x;
    min.y *= scale.y;
    min.z *= scale.z;
    min += position;

    max.x *= scale.x;
    max.y *= scale.y;
    max.z *= scale.z;
    max += position;
}
BoundingBox::BoundingBox(vec3 min, vec3 max) : min(min), max(max) {}

bool BoundingBox::overlaps(BoundingBox box) {
    if (((min.x <= box.max.x ) && (box.min.x <= max.x ) &&
       (min.y <= box.max.y ) && (box.min.y <= max.y ) &&
       (min.z <= box.max.z ) && (box.min.z <= max.z ))) {
        return true;
    } else {
        return false;
    }
}

bool BoundingBox::contains(vec3 p) {
    if (p.x >= min.x -EPS && p.y >= min.y -EPS&& p.z >= min.z -EPS&& p.x <= max.x +EPS && p.y <= max.y +EPS && p.z <= max.z+EPS) {
        return true;
    } else {
        return false;
    }
}

double BoundingBox::intersectPlane(vec3 p, vec3 n, Ray ray) {
    double np = ray.direction.dot(n);
    return (p.dot(n)-ray.origin.dot(n))/np;
}

std::vector<vec3> BoundingBox::intersect(Ray ray) {

    double t1 = 0;
    std::vector<vec3> intersections;

    for (int i = 0; i < 3; i++) {
        double t = intersectPlane(min, vec3(i == 0, i == 1, i == 2), ray);
        vec3 intersection = ray.origin + ray.direction*t;
        if (t > EPS && contains(intersection)) {
            if (t > t1) {
                t1 = t;
                intersections.push_back(intersection);
            } else
                intersections.insert(intersections.begin(), intersection);
        }
    }

    for (int i = 0; i < 3; i++) {
        double t = intersectPlane(max, vec3(-(i == 0), -(i == 1), -(i == 2)), ray);
        vec3 intersection = ray.origin + ray.direction*t;
        if (t > EPS && contains(intersection)) {
            if (t > t1) {
                t1 = t;
                intersections.push_back(intersection);
            } else
                intersections.insert(intersections.begin(), intersection);
        }
    }
    

    if (contains(ray.origin)) {
        intersections.insert(intersections.begin(), ray.origin);
    }


    return intersections;
    //return std::pair<nullptr, nullptr>;


/*     double t = Plane::Intersect(ray, intersection);
    vec3 d = intersection - p0;
    vec2 dist = vec2(d.dot(left), d.dot(up));
    if (std::abs(dist.x) <= size.x && std::abs(dist.y) <= size.y) {
        return t; 
    } else {
        return -1;
    } */
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

    return BoundingBox(vec3(minX,minY,minZ),
                       vec3(maxX,maxY,maxZ));
}

Shape::Shape(vec3 p0) : p0(p0), isLight(false), objectToWorld(Transform(vec3(0,0,0), vec3(1,1,1), vec3())), aabb(BoundingBox(vec3(),vec3())) {

}

Plane::Plane(vec3 p0, vec3 n) : Shape(p0), n(n) {
    if (std::abs(n.x) > std::abs(n.y)) {
		double invLen = 1.f / sqrt(n.x * n.x + n.z * n.z);
		up = vec3(-n.z * invLen, 0.0f, n.x * invLen);
    } else {
		double invLen = 1.0f / sqrt(n.y * n.y + n.z * n.z);
		up = vec3(0.0f, n.z * invLen, -n.y * invLen);
    }
    left = n.cross(up);
}

vec3 Plane::normal(vec3 p) {
    return n;
}

double Plane::Intersect(Ray ray) {

    double np = ray.direction.dot(n);
    if (np > 0){
        return 0;
    }
    double t = (p0.dot(n)-ray.origin.dot(n))/np;

    return t;
}

BoundingBox Plane::calculateBoundingBox() {
    return BoundingBox(vec3(), vec3());
}

void Plane::transform(vec3 position, vec3 scale) {
    p0 += position;
}



Disc::Disc(vec3 p0, vec3 n, vec2 size) : Plane(p0, n), size(size) {
    area = calculateArea();
    rotationMatrix = n.rotationMatrix();
}

double Disc::Intersect(Ray ray) {
    double t = Plane::Intersect(ray);
    vec3 intersection = ray.origin + ray.direction*t;
    if ((intersection - p0).length() < size.x) {
        return t;
    } else {
        return -1;
    }
}

BoundingBox Disc::calculateBoundingBox() {
    return BoundingBox(vec3(), vec3());
}

void Disc::transform(vec3 position, vec3 scale) {
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

    vec3 pos = p0 + left*x + up*y;

	return Sample3D(pos, 1/area);
}



Quad::Quad(vec3 p0, vec3 n, vec2 size, vec3 up) : Plane(p0, n), size(size) {
    this->up = up;
    left = up.cross(n);
}

vec2 planarUVMapping(vec3 pos, vec3 p0, vec3 left, vec3 up, vec2 size) {
    vec3 d = pos - p0;
    double x = (d.dot(left) + size.x)/(size.x*2);
    double y = (d.dot(up) + size.y)/(size.y*2);
    if (x > 1)
        x = 1;
    if (y > 1)
        y = 1;
    return vec2(x, y);
}

std::function<vec2(vec3)> Shape::defaultUVMapping() {
    return [](vec3 v) {return vec2(0,0);};
}

std::function<vec2(vec3)> Quad::defaultUVMapping() {
    using namespace std::placeholders; 
    return std::bind(planarUVMapping, _1, p0, left, up, size);
}

std::function<vec2(vec3)> Disc::defaultUVMapping() {
    using namespace std::placeholders; 
    return std::bind(planarUVMapping, _1, p0, left, up, size);
}

BoundingBox Quad::calculateBoundingBox() {
    return BoundingBox(vec3(), vec3());
}

double Quad::Intersect(Ray ray) {
    double t = Plane::Intersect(ray);
    vec3 intersection = ray.origin + ray.direction*t;
    vec3 d = intersection - p0;
    vec2 dist = vec2(d.dot(left), d.dot(up));
    if (std::abs(dist.x) <= size.x && std::abs(dist.y) <= size.y) {
        return t; 
    } else {
        return -1;
    }
}

void Quad::transform(vec3 position, vec3 scale) {
    p0 += position;
    size.x *= scale.x;
    size.y *= scale.y;
}


Sphere::Sphere(vec3 p0, double r) : Shape(p0), radius(r) {}

double Sphere::Intersect(Ray ray) {
    vec3 dir2 = ray.origin-p0;
    double b = (dir2*2).dot(ray.direction);
    double c = dir2.dot(dir2) - (radius*radius);
    double d = b*b - 4*c;
    if (d<0) {
        return -1;
    }
    d = std::sqrt(d);
    double s1 = -b + d;
    double s2 = -b - d;
    
    double t = (s2>EPS) ? s2/2 : ((s1>EPS) ? s1/2 : 0);
  
    return t;

}

void Sphere::transform(vec3 position, vec3 scale) {
    p0 += position;
    radius *= scale.x;
}

BoundingBox Sphere::calculateBoundingBox() {
    return BoundingBox(vec3(), vec3());
}

vec3 Sphere::normal(vec3 p) {
    return (p - p0).norm();
}

Hemisphere::Hemisphere(vec3 p0, double r) : Shape(p0), radius(r) {}

Sample3D Hemisphere::map(Sample2D sample) {
    const double r = radius*std::sqrt(sample.value.x);
	const double theta = TAU * sample.value.y;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);

    double height = std::sqrt(1-r*r);
    
	return Sample3D(vec3(x, y, height), sample.pdf*height*IPI);
}

double Hemisphere::Intersect(Ray ray) {
    vec3 dir2 = ray.origin-p0;
    double b = (dir2*2).dot(ray.direction);
    double c = dir2.dot(dir2) - (radius*radius);
    double d = b*b - 4*c;
    if (d<0) {
        return -1;
    }
    d = std::sqrt(d);
    double s1 = -b + d;
    double s2 = -b - d;
    
    double t = (s2>EPS) ? s2/2 : ((s1>EPS) ? s1/2 : 0);

    return t;

}

void Hemisphere::transform(vec3 position, vec3 scale) {
    p0 += position;
    radius *= scale.x;
}

BoundingBox Hemisphere::calculateBoundingBox() {
    return BoundingBox(vec3(), vec3());
}

vec3 Hemisphere::normal(vec3 p) {
    return (p - p0).norm();
}


