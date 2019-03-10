#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/sample.h"
#include "../samplers/sampler.h"
#include <functional>
#include "../core/image.h"
#include "geometry.h"

class Material {
public:
    using Mapping = std::function<Vector3(Vector3)>;
    using UVMapping = std::function<Vector2(Vector3)>;
    Material();
    Vector3 albedo;
    Vector3 emission;
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector3 p);
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n) = 0;
    virtual SampleBSDF sample(Vector3 dir, Vector3 n, Sampler* sampler) = 0;
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n) = 0;
    virtual Vector3 reflectance(Vector3 dir, Vector3 odir, Vector3 n);
    virtual void bind(Shape* geometry);
    std::function<Vector2(Vector3)> uvMapping;
    virtual Vector3 getAlbedo(Vector3 p);
    Image texture;

    bool textured = false;
};