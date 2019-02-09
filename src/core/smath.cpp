#include "smath.h"


Transform::Transform(Vector3 p, Vector3 s, Vector3 r) {
    inverse = setMatrix((Vector3()-p)*Vector3(1/s.x, 1/s.y, 1/s.z),Vector3(1/s.x, 1/s.y, 1/s.z),Vector3()-r);
    matrix = setMatrix(p, s, r);
}

Matrix4 Transform::setMatrix(Vector3 p, Vector3 s, Vector3 r) {
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



Vector3 Transform::apply(Vector3 v) {
    Vector4 v4 = matrix*Vector4(v, 1);
    return Vector3(v4.x, v4.y, v4.z);
}

Vector3 Transform::applyInverse(Vector3 v) {
    Vector4 v4 = inverse*Vector4(v, 1);
    return Vector3(v4.x, v4.y, v4.z);
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


Vector3 Matrix3::operator*(Vector3 v) const {
    Vector3 result;

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

Vector3 Matrix3::column(int i) const {
    return Vector3(mat[0][i], mat[1][i], mat[2][i]);
}

Vector3 Matrix3::row(int i) const {
    return Vector3(mat[i][0], mat[i][1], mat[i][2]);
}

Matrix3 Vector3::rotationMatrix() {
    Vector3 b(0, 0, 1);

    Vector3 test = this->cross(b);

    Matrix3 v = this->cross(b).norm().skewSymmetric();

    double s = this->length();

    double c = this->dot(b);


    Matrix3 identity(1);

    Matrix3 rotation = identity + v + v*v*(1-c)*(1/(s*s));

    return rotation;

}

Vector3 Vector3::rotate(Vector3 &v) const {
    Vector3 up;
    double vx2 = v.x*v.x;
    double vy2 = v.y*v.y;
    double vz2 = v.z*v.z;
 

    if (vx2 > vy2) {
		double invLen = 1.f / std::sqrtf(vx2+ vz2);
		up = Vector3(-v.z * invLen, 0.0f, v.x * invLen);
    } else {
		double invLen = 1.0f / std::sqrtf(vy2 + vz2);
		up = Vector3(0.0f, v.z * invLen, -v.y * invLen);
    }
    Vector3 left = v.cross(up);

    Vector3 rotatedDir;
	rotatedDir.x = Vector3(up.x, 
                           left.x, 
                           v.x).dot(*this);

	rotatedDir.y = Vector3(up.y, 
                           left.y, 
                           v.y).dot(*this);

	rotatedDir.z = Vector3(up.z, 
                           left.z, 
                           v.z).dot(*this);
	
    return rotatedDir;
}

Matrix3 Vector3::skewSymmetric() {
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
    if (len == 0 ) {
        return Vector3();
    } else {
        return Vector3(x/len, y/len, z/len);
    }
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