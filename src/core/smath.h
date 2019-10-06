#pragma once
#include <cmath>
#include <vector>

const double PI = 3.141592653589793238463;
const double INF = 1000000000;
const double TAU = 2*PI;
const double IPI = 1/PI;
const double EPS = 0.000000001;

struct vec3;

struct Matrix3 {
    std::vector<std::vector<double>> mat;
    Matrix3();
    Matrix3(int i);
    void set(int row, int col, double value);
    double get(int row, int col);
    Matrix3 operator*(Matrix3 m) const;
    vec3 operator*(vec3 v) const;
    Matrix3 operator*(double f) const; 
    Matrix3 operator+(Matrix3 m) const;
    vec3 row(int i) const;
    vec3 column(int i) const;
};


struct vec3 {
    double x, y, z;
    vec3();
    vec3(double x, double y, double z);
    double luminance() const;
    double dot(const vec3 &v) const;
    vec3 cross(const vec3 &v);
    vec3 rotate(vec3 &v) const; 
    vec3 operator+(const vec3 &v) const;
    vec3 operator+=(const vec3 &v);
    vec3 operator-(const vec3 &v) const;
    vec3 operator-=(const vec3 &v);
    bool operator==(const vec3 &v) const;
    bool operator!=(const vec3 &v) const;
    vec3 operator*(double f) const;
    vec3 operator*=(double f);
    vec3 operator*(vec3 v) const;
    vec3 operator/(double f) const;
    vec3 operator/=(double f);
    double length() const;
    vec3 norm();
    vec3 clamp();
    Matrix3 skewSymmetric();
    Matrix3 rotationMatrix();
};

struct vec2 {
    double x, y;
    vec2();
    vec2(double x, double y);
    vec2 operator+(const vec2 &v) const;
    vec2 operator+=(const vec2 &v);
    vec2 operator-(const vec2 &v) const;
    vec2 operator-=(const vec2 &v);
    bool operator==(const vec2 &v) const;
    bool operator!=(const vec2 &v) const;
    vec2 operator*(double f) const;
    vec2 operator*=(double f);
    vec2 operator/(double f) const;
    vec2 operator/=(double f);
    double length() const;
};






struct Vector4 {
    double x, y, z, w;
    Vector4();
    Vector4(vec3 v, double w);
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
    Matrix4 setMatrix(vec3 p, vec3 s, vec3 r);
    Transform() {};
    Transform(vec3 p, vec3 s, vec3 r);
    vec3 apply(vec3 v);
    vec3 applyInverse(vec3 v);
};


