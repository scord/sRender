#include "smath.h"

Transform::Transform(Vector3 p, Vector3 r, Vector3 s) {
    Matrix4 matrixTS;

    matrixTS.set(0, 0, s.x);
    matrixTS.set(1, 1, s.y);
    matrixTS.set(2, 2, s.z);
    matrixTS.set(3, 3, 1);
    matrixTS.set(0, 3, p.x);
    matrixTS.set(1, 3, p.y);
    matrixTS.set(2, 3, p.z);

    Matrix4 matrixRx;

    matrixRx.set(0, 0, 1);
    matrixRx.set(1, 1, cos(r.x));
    matrixRx.set(1, 2, -sin(r.x));
    matrixRx.set(2, 1, sin(r.x));
    matrixRx.set(2, 2, cos(r.x));
    matrixRx.set(3, 3, 1);

    Matrix4 matrixRy;

    matrixRx.set(1, 1, 1);
    matrixRx.set(0, 0, cos(r.y));
    matrixRx.set(0, 2, -sin(r.y));
    matrixRx.set(2, 0, sin(r.y));
    matrixRx.set(2, 2, cos(r.y));
    matrixRx.set(3, 3, 1);

    Matrix4 matrixRz;

    matrixRx.set(2, 2, 1);
    matrixRx.set(0, 0, cos(r.x));
    matrixRx.set(0, 1, -sin(r.x));
    matrixRx.set(1, 0, sin(r.x));
    matrixRx.set(1, 1, cos(r.x));
    matrixRx.set(3, 3, 1);

    matrix = matrixTS * matrixRx * matrixRy * matrixRz;
}

Vector3 Transform::apply(Vector3 v) {
    Vector4 v4 = matrix*Vector4(v, 1);
    return Vector3(v4.x, v4.y, v4.z);
}

Matrix4::Matrix4() {
    mat = std::vector<std::vector<double>> {{0,0,0,0},
                                           {0,0,0,0},
                                           {0,0,0,0},
                                           {0,0,0,0}};
}

void Matrix4::set(int row, int col, double value) {
    mat[row][col] = value;
}

Vector4 Matrix4::operator*(Vector4 v) const {
    Vector4 result;

    result.x = this->row(0).dot(v);
    result.y = this->row(1).dot(v);
    result.z = this->row(1).dot(v);
    result.w = 1;

    return result;

}

Matrix4 Matrix4::operator*(Matrix4 m) const {
    Matrix4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->row(i).dot(m.column(j)));
        }
    }

    return result;
}

Vector4 Matrix4::column(int i) const {
    return Vector4(mat[0][i], mat[1][i], mat[2][i], mat[3][i]);
}

Vector4 Matrix4::row(int i) const {
    return Vector4(mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
}

double Vector4::dot(const Vector4 &v) const {
    return v.x * x + v.y * y + v.z * z + v.w * w;
}

Vector4::Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Vector4::Vector4(Vector3 v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

Vector4::Vector4() {
    x = y = z = w = 0.f;
}

Vector3::Vector3() {
    x = y = z = 0.f;
}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

double Vector3::dot(const Vector3 &v) {
    return x*v.x + y*v.y + z*v.z;
}

Vector3 Vector3::cross(const Vector3 &v) {
    return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

Vector3 Vector3::operator+(const Vector3 &v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator+=(const Vector3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3 &v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-=(const Vector3 &v){
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

bool Vector3::operator==(const Vector3 &v) const {
    return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!=(const Vector3 &v) const {
    return x != v.x || y != v.y || z != v.z;
}

Vector3 Vector3::operator*(double f) const {
    return Vector3(f*x, f*y, f*z);
}

Vector3 Vector3::operator*=(double f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

Vector3 Vector3::operator*(Vector3 v) const {
    return Vector3(x*v.x, y*v.y, z*v.z);
}

Vector3 Vector3::operator/(double f) const {
    return Vector3(x/f, y/f, z/f);
}

Vector3 Vector3::operator/=(double f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

double Vector3::length() const { 
    return std::sqrt(x * x + y * y + z * z);
}  

Vector3 Vector3::norm() {
    double len = length();
    return Vector3(x/len, y/len, z/len);
}

Vector3 Vector3::clamp() {
    double a,b,c;
    if (x > 1.0) {
        a = 1.0;
    } else {
        a = x;
    }
    if (y > 1.0) {
        b = 1.0;
    } else {
        b = y;
    }
    if (z > 1.0) {
        c = 1.0;
    } else {
        c = z;
    }
    return Vector3(a, b, c);
}

Vector2::Vector2() {
        x = y = 0.f;
    }

Vector2::Vector2(double x, double y): x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2 &v) const {
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+=(const Vector2 &v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector2 Vector2::operator-(const Vector2 &v) const {
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-=(const Vector2 &v){
    x -= v.x;
    y -= v.y;
    return *this;
}

bool Vector2::operator==(const Vector2 &v) const {
    return x == v.x && y == v.y;
}

bool Vector2::operator!=(const Vector2 &v) const {
    return x != v.x || y != v.y;
}

Vector2 Vector2::operator*(double f) const {
    return Vector2(f*x, f*y);
}

Vector2 Vector2::operator*=(double f) {
    x *= f;
    y *= f;
    return *this;
}

Vector2 Vector2::operator/(double f) const {
    return Vector2(x/f, y/f);
}

Vector2 Vector2::operator/=(double f) {
    x /= f;
    y /= f;
    return *this;
}

double Vector2::length() const { 
    return std::sqrt(x * x + y * y);
}

Ray::Ray(Vector3 origin, Vector3 direction):origin(origin), direction(direction) {}
//0,0,-1

void Ray::rotateToVector(Vector3 v) {
    Vector3 up;
    if (std::abs(v.x) > std::abs(v.y)) {
		double invLen = 1.f / sqrtf(v.x * v.x + v.z * v.z);
		up = Vector3(-v.z * invLen, 0.0f, v.x * invLen);
    } else {
		double invLen = 1.0f / sqrtf(v.y * v.y + v.z * v.z);
		up = Vector3(0.0f, v.z * invLen, -v.y * invLen);
    }
    Vector3 left = v.cross(up);

    Vector3 rotatedDir;
	rotatedDir.x = Vector3(up.x, 
                           left.x, 
                           v.x).dot(direction);

	rotatedDir.y = Vector3(up.y, 
                           left.y, 
                           v.y).dot(direction);

	rotatedDir.z = Vector3(up.z, 
                           left.z, 
                           v.z).dot(direction);
	
    direction = rotatedDir;
}