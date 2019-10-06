#include "smath.h"


Transform::Transform(vec3 p, vec3 s, vec3 r) {
    inverse = setMatrix((vec3()-p)*vec3(1/s.x, 1/s.y, 1/s.z),vec3(1/s.x, 1/s.y, 1/s.z),vec3()-r);
    matrix = setMatrix(p, s, r);
}

Matrix4 Transform::setMatrix(vec3 p, vec3 s, vec3 r) {
    Matrix4 matrixTS;

    matrixTS.set(0, 0, s.x);
    matrixTS.set(1, 1, s.y);
    matrixTS.set(2, 2, s.z);
    matrixTS.set(3, 3, 1);
    matrixTS.set(0, 3, p.x);
    matrixTS.set(1, 3, p.y);
    matrixTS.set(2, 3, p.z);

    return matrixTS;
}



vec3 Transform::apply(vec3 v) {
    Vector4 v4 = matrix*Vector4(v, 1);
    return vec3(v4.x, v4.y, v4.z);
}

vec3 Transform::applyInverse(vec3 v) {
    Vector4 v4 = inverse*Vector4(v, 1);
    return vec3(v4.x, v4.y, v4.z);
}

Matrix3::Matrix3() {
    mat = std::vector<std::vector<double>> {{0,0,0},
                                                {0,0,0},
                                                {0,0,0}};
}

Matrix3::Matrix3(int i) {

    if (i == 0) {

        mat = std::vector<std::vector<double>> {{0,0,0},
                                                {0,0,0},
                                                {0,0,0}};
    } else {
        mat = std::vector<std::vector<double>> {{1,0,0},
                                                {0,1,0},
                                                {0,0,1}};
    }
}

void Matrix3::set(int row, int col, double value) {
    mat[row][col] = value;
}

double Matrix3::get(int row, int col) {
    return mat[row][col];
}


vec3 Matrix3::operator*(vec3 v) const {
    vec3 result;

    result.x = this->row(0).dot(v);
    result.y = this->row(1).dot(v);
    result.z = this->row(2).dot(v);

    return result;

}


Matrix3 Matrix3::operator*(double f) const {
    Matrix3 result(0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, mat[i][j]*f);
        }
    }

    return result;
}

Matrix3 Matrix3::operator*(Matrix3 m) const {
    Matrix3 result(0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->row(i).dot(m.column(j)));
        }
    }

    return result;
}

Matrix3 Matrix3::operator+(Matrix3 m) const {
    Matrix3 result(0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, mat[i][j] + m.get(i, j));
        }
    }

    return result;
}

vec3 Matrix3::column(int i) const {
    return vec3(mat[0][i], mat[1][i], mat[2][i]);
}

vec3 Matrix3::row(int i) const {
    return vec3(mat[i][0], mat[i][1], mat[i][2]);
}

Matrix3 vec3::rotationMatrix() {
    vec3 b(0, 0, 1);

    Matrix3 v = this->cross(b).norm().skewSymmetric();

    double c = this->dot(b);
    double s = std::sqrt(1-c*c);

    Matrix3 identity(1);

    Matrix3 rotation = identity + v + v*v*(1-c)*(1/(s*s));

    return rotation;

}

vec3 vec3::rotate(vec3 &v) const {
    vec3 up;
    double vx2 = v.x*v.x;
    double vy2 = v.y*v.y;
    double vz2 = v.z*v.z;
 

    if (vx2 > vy2) {
		double invLen = 1.f / std::sqrt(vx2+ vz2);
		up = vec3(-v.z * invLen, 0.0f, v.x * invLen);
    } else {
		double invLen = 1.0f / std::sqrt(vy2 + vz2);
		up = vec3(0.0f, v.z * invLen, -v.y * invLen);
    }
    vec3 left = v.cross(up);

    vec3 rotatedDir;
	rotatedDir.x = vec3(up.x, 
                           left.x, 
                           v.x).dot(*this);

	rotatedDir.y = vec3(up.y, 
                           left.y, 
                           v.y).dot(*this);

	rotatedDir.z = vec3(up.z, 
                           left.z, 
                           v.z).dot(*this);
	
    return rotatedDir;
}

Matrix3 vec3::skewSymmetric() {
    Matrix3 mat(0);
    mat.set(0, 1, -z);
    mat.set(0, 2, y);
    mat.set(1, 0, z);
    mat.set(1, 2, -x);
    mat.set(2, 0, -y);
    mat.set(2,1, x);
    return mat;
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
    result.z = this->row(2).dot(v);
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

Vector4::Vector4(vec3 v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

Vector4::Vector4() {
    x = y = z = w = 0.f;
}

vec3::vec3() {
    x = y = z = 0.f;
}

double vec3::luminance() const {
    return 0.299*x + 0.587*y + 0.114*z;
}

vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {}

double vec3::dot(const vec3 &v) const {
    return x*v.x + y*v.y + z*v.z;
}

vec3 vec3::cross(const vec3 &v) {
    return vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

vec3 vec3::operator+(const vec3 &v) const {
    return vec3(x + v.x, y + v.y, z + v.z);
}


vec3 vec3::operator+=(const vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vec3 vec3::operator-(const vec3 &v) const {
    return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator-=(const vec3 &v){
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}


bool vec3::operator==(const vec3 &v) const {
    return x == v.x && y == v.y && z == v.z;
}

bool vec3::operator!=(const vec3 &v) const {
    return x != v.x || y != v.y || z != v.z;
}

vec3 vec3::operator*(double f) const {
    return vec3(f*x, f*y, f*z);
}

vec3 vec3::operator*=(double f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

vec3 vec3::operator*(vec3 v) const {
    return vec3(x*v.x, y*v.y, z*v.z);
}

vec3 vec3::operator/(double f) const {
    return vec3(x/f, y/f, z/f);
}

vec3 vec3::operator/=(double f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

double vec3::length() const { 
    return std::sqrt(x * x + y * y + z * z);
}  

vec3 vec3::norm() {
    double len = length();
    if (len == 0 ) {
        return vec3();
    } else {
        return vec3(x/len, y/len, z/len);
    }
}

vec3 vec3::clamp() {
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
    return vec3(a, b, c);
}

vec2::vec2() {
        x = y = 0.f;
    }

vec2::vec2(double x, double y): x(x), y(y) {}

vec2 vec2::operator+(const vec2 &v) const {
    return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator+=(const vec2 &v) {
    x += v.x;
    y += v.y;
    return *this;
}

vec2 vec2::operator-(const vec2 &v) const {
    return vec2(x - v.x, y - v.y);
}

vec2 vec2::operator-=(const vec2 &v){
    x -= v.x;
    y -= v.y;
    return *this;
}

bool vec2::operator==(const vec2 &v) const {
    return x == v.x && y == v.y;
}

bool vec2::operator!=(const vec2 &v) const {
    return x != v.x || y != v.y;
}

vec2 vec2::operator*(double f) const {
    return vec2(f*x, f*y);
}

vec2 vec2::operator*=(double f) {
    x *= f;
    y *= f;
    return *this;
}

vec2 vec2::operator/(double f) const {
    return vec2(x/f, y/f);
}

vec2 vec2::operator/=(double f) {
    x /= f;
    y /= f;
    return *this;
}

double vec2::length() const { 
    return std::sqrt(x * x + y * y);
}
