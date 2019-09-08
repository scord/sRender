#include "geometry.h"
#include "../core/smath.h"
#include <cmath>
#include <iostream>
#include <cassert>
#include "math/smath2.h"

Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2) : Shape(v0), v1(v1), v2(v2) {
    n = ((v1 - v0).norm()).cross((v2 - v0).norm()).norm();
    aabb = calculateBoundingBox();
}

Vector3 Triangle::samplePoint() {
    return Vector3();
}
Vector3 Disc::samplePoint() {
    return Vector3();   
}
Vector3 Quad::samplePoint() {
    return Vector3();    
}
Vector3 Sphere::samplePoint() {
    return Vector3();    
}
Vector3 Hemisphere::samplePoint() {
    return Vector3();    
}
Vector3 Plane::samplePoint() {
    return Vector3();    
}



Vector3 Triangle::normal(Vector3 p) {
    return n;
}


double Triangle::Intersect(Ray ray) {


    double np = ray.direction.dot(n);
    double t = (p0.dot(n)-ray.origin.dot(n))/np;
    Vector3 intersection = ray.origin + ray.direction*t;
    
    if (n.dot((v1 - p0).cross(intersection - p0)) < 0) return -1;
    if (n.dot((v2 - v1).cross(intersection - v1)) < 0) return -1;
    if (n.dot((p0 - v2).cross(intersection - v2)) < 0) return -1;

    return t;
}

void Triangle::transform(Vector3 position, Vector3 scale) {


/*     p0 = rotateVector(p0, Vector3(0,0,0), Vector3(0,1,0), 0.9f);
    v1 = rotateVector(v1, Vector3(0,0,0), Vector3(0,1,0), 0.9f);
    v2 = rotateVector(v2, Vector3(0,0,0), Vector3(0,1,0), 0.9f);
 */
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


    aabb = calculateBoundingBox();
  
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
    if (((min.x <= box.max.x ) && (box.min.x <= max.x ) &&
       (min.y <= box.max.y ) && (box.min.y <= max.y ) &&
       (min.z <= box.max.z ) && (box.min.z <= max.z ))) {
        return true;
    } else {
        return false;
    }
}

bool BoundingBox::contains(Vector3 p) {
    if (p.x >= min.x -EPS && p.y >= min.y -EPS&& p.z >= min.z -EPS&& p.x <= max.x +EPS && p.y <= max.y +EPS && p.z <= max.z+EPS) {
        return true;
    } else {
        return false;
    }
}

double BoundingBox::intersectPlaneG(const PGA3D& plane, const PGA3D& ray, const Vector3& dir, const Vector3& p, Vector3& intersection) {



    PGA3D intersection_point = ray ^ plane;
    intersection = pointToVector(intersection_point);
    Vector3 dir2 = intersection - p;

    PGA3D intersection_line = toPoint(p) & intersection_point;

    PGA3D dottest = ray*intersection_line;


    if (dir.dot(dir2) >= 0 )
        return  dir2.length();
    else {
        return -dir2.length();
    }

    

    
   // return (p.dot(n)-ray.origin.dot(n))/np;
}

double BoundingBox::intersectPlane(Vector3 p, Vector3 n, Ray ray) {

    PGA3D pp = toPoint(p);
    PGA3D l = toLine(p, p+n);

    PGA3D plane = (l | pp);

    PGA3D rayl = toPoint(ray.origin) & toPoint(ray.origin + ray.direction);
    PGA3D intersection_point = rayl ^ plane;
    PGA3D intersection_line = toPoint(ray.origin) & intersection_point;
    Vector3 dir = pointToVector(intersection_point) - ray.origin;

    PGA3D dottest = rayl*intersection_point;


    if (ray.direction.dot(dir) >= 0 )
        return  dir.length();
    else {
        return -dir.length();
    }

    

    
   // return (p.dot(n)-ray.origin.dot(n))/np;
}

std::vector<Vector3> BoundingBox::intersect(Ray ray) {

    double t1 = 0;
    std::vector<Vector3> intersections;

    PGA3D min2 = point(min.x, min.y, min.z);
    PGA3D max2 = point(max.x, max.y, max.z);
    PGA3D rayl = toLine(ray.origin, ray.origin+ray.direction);
    

    for (int i = 0; i < 3; i++) {
        Vector3 intersection;
        double t = intersectPlaneG((min2 & toPoint(min+Vector3(i == 0, i == 1, i == 2)) | min2), rayl, ray.direction, ray.origin, intersection);

        if (t > EPS && contains(intersection)) {
            if (t > t1) {
                t1 = t;
                intersections.push_back(intersection);
            } else
                intersections.insert(intersections.begin(), intersection);
        }
    }

    for (int i = 0; i < 3; i++) {
        Vector3 intersection;
        double t = intersectPlaneG((max2 & toPoint(max+Vector3(-(i == 0), -(i == 1), -(i == 2))) | max2), rayl, ray.direction, ray.origin, intersection);
       
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
    Vector3 d = intersection - p0;
    Vector2 dist = Vector2(d.dot(left), d.dot(up));
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

    return BoundingBox(Vector3(minX,minY,minZ),
                       Vector3(maxX,maxY,maxZ));
}

Shape::Shape(Vector3 p0) : p0(p0), isLight(false), objectToWorld(Transform(Vector3(0,0,0), Vector3(1,1,1), Vector3())), aabb(BoundingBox(Vector3(),Vector3())) {

}

Plane::Plane(Vector3 p0, Vector3 n) : Shape(p0), n(n) {
    if (std::abs(n.x) > std::abs(n.y)) {
		double invLen = 1.f / sqrt(n.x * n.x + n.z * n.z);
		up = Vector3(-n.z * invLen, 0.0f, n.x * invLen);
    } else {
		double invLen = 1.0f / sqrt(n.y * n.y + n.z * n.z);
		up = Vector3(0.0f, n.z * invLen, -n.y * invLen);
    }
    left = n.cross(up);
}

Vector3 Plane::normal(Vector3 p) {
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
    return BoundingBox(Vector3(), Vector3());
}

void Plane::transform(Vector3 position, Vector3 scale) {
    p0 += position;
}



Disc::Disc(Vector3 p0, Vector3 n, Vector2 size) : Plane(p0, n), size(size) {
    area = calculateArea();
    rotationMatrix = n.rotationMatrix();
}

double Disc::Intersect(Ray ray) {
    double t = Plane::Intersect(ray);
    Vector3 intersection = ray.origin + ray.direction*t;
    if ((intersection - p0).length() < size.x) {
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

    Vector3 pos = p0 + left*x + up*y;

	return Sample3D(pos, 1/area);
}



Quad::Quad(Vector3 p0, Vector3 n, Vector2 size, Vector3 up) : Plane(p0, n), size(size) {
    this->up = up;
    left = up.cross(n);
}

Vector2 planarUVMapping(Vector3 pos, Vector3 p0, Vector3 left, Vector3 up, Vector2 size) {
    Vector3 d = pos - p0;
    double x = (d.dot(left) + size.x)/(size.x*2);
    double y = (d.dot(up) + size.y)/(size.y*2);
    if (x > 1)
        x = 1;
    if (y > 1)
        y = 1;
    return Vector2(x, y);
}

std::function<Vector2(Vector3)> Shape::defaultUVMapping() {
    return [](Vector3 v) {return Vector2(0,0);};
}

std::function<Vector2(Vector3)> Quad::defaultUVMapping() {
    using namespace std::placeholders; 
    return std::bind(planarUVMapping, _1, p0, left, up, size);
}

std::function<Vector2(Vector3)> Disc::defaultUVMapping() {
    using namespace std::placeholders; 
    return std::bind(planarUVMapping, _1, p0, left, up, size);
}

BoundingBox Quad::calculateBoundingBox() {
    return BoundingBox(Vector3(), Vector3());
}

double Quad::Intersect(Ray ray) {
    double t = Plane::Intersect(ray);
    Vector3 intersection = ray.origin + ray.direction*t;
    Vector3 d = intersection - p0;
    Vector2 dist = Vector2(d.dot(left), d.dot(up));
    if (std::abs(dist.x) <= size.x && std::abs(dist.y) <= size.y) {
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


Sphere::Sphere(Vector3 p0, double r) : Shape(p0), radius(r) {}

double Sphere::Intersect(Ray ray) {
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
    
    double t = (s2>EPS) ? s2/2 : ((s1>EPS) ? s1/2 : 0);
  
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

Hemisphere::Hemisphere(Vector3 p0, double r) : Shape(p0), radius(r) {}

Sample3D Hemisphere::map(Sample2D sample) {
    const double r = radius*std::sqrt(sample.value.x);
	const double theta = TAU * sample.value.y;

	const double x = r * std::cos(theta);
	const double y = r * std::sin(theta);

    double height = std::sqrt(1-r*r);
    
	return Sample3D(Vector3(x, y, height), sample.pdf*height*IPI);
}

double Hemisphere::Intersect(Ray ray) {
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
    
    double t = (s2>EPS) ? s2/2 : ((s1>EPS) ? s1/2 : 0);

    return t;

}

void Hemisphere::transform(Vector3 position, Vector3 scale) {
    p0 += position;
    radius *= scale.x;
}

BoundingBox Hemisphere::calculateBoundingBox() {
    return BoundingBox(Vector3(), Vector3());
}

Vector3 Hemisphere::normal(Vector3 p) {
    return (p - p0).norm();
}


