#pragma once
#include <cmath>
#include <vector>

const double PI = 3.141592653589793238463;
const double TAU = 2*PI;
const double IPI = 1/PI;

struct Vector3 {
    double x, y, z;
    Vector3();
    Vector3(double x, double y, double z);
    double dot(const Vector3 &v);
    Vector3 cross(const Vector3 &v);
    Vector3 operator+(const Vector3 &v) const;
    Vector3 operator+=(const Vector3 &v);
    Vector3 operator-(const Vector3 &v) const;
    Vector3 operator-=(const Vector3 &v);
    bool operator==(const Vector3 &v) const;
    bool operator!=(const Vector3 &v) const;
    Vector3 operator*(double f) const;
    Vector3 operator*=(double f);
    Vector3 operator*(Vector3 v) const;
    Vector3 operator/(double f) const;
    Vector3 operator/=(double f);
    double length() const;
    Vector3 norm();
    Vector3 clamp();
};

struct Vector2 {
    double x, y;
    Vector2();
    Vector2(double x, double y);
    Vector2 operator+(const Vector2 &v) const;
    Vector2 operator+=(const Vector2 &v);
    Vector2 operator-(const Vector2 &v) const;
    Vector2 operator-=(const Vector2 &v);
    bool operator==(const Vector2 &v) const;
    bool operator!=(const Vector2 &v) const;
    Vector2 operator*(double f) const;
    Vector2 operator*=(double f);
    Vector2 operator/(double f) const;
    Vector2 operator/=(double f);
    double length() const;
};

struct Ray {
    Vector3 origin;
    Vector3 direction;
    Ray(Vector3 origin, Vector3 direction);
    void rotateToVector(Vector3 v);
};




struct Vector4 {
    double x, y, z, w;
    Vector4();
    Vector4(Vector3 v, double w);
    double dot(const Vector4 &v) const;
    Vector4(double x, double y, double z, double w);
};

struct Matrix4 {
    std::vector<std::vector<double>> mat;
    Matrix4();
    void set(int row, int col, double value);
    Matrix4 operator*(Matrix4 m) const;
    Vector4 operator*(Vector4 v) const;
    Vector4 row(int i) const;
    Vector4 column(int i) const;
};

struct Transform {
    Matrix4 matrix;
    Matrix4 inverse;
    Matrix4 setMatrix(Vector3 p, Vector3 s, Vector3 r);
    Transform() {};
    Transform(Vector3 p, Vector3 s, Vector3 r);
    Vector3 apply(Vector3 v);
    Vector3 applyInverse(Vector3 v);
};


