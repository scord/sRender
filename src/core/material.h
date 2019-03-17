#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/sample.h"
#include "../samplers/sampler.h"
#include <functional>
#include "../core/texture.h"
#include "geometry.h"
#include "texture.h"


template <class T>
class MaterialProperty {
public:
    
    MaterialProperty() {
        value_ = [](Vector2& uv){return T();};
    }

    MaterialProperty(T value) {
        uniformValue = value;
        value_ = [&](Vector2& uv){return uniformValue;};
    }

    MaterialProperty(Texture& tex) {
        texture = tex;
        value_ = [&](Vector2& uv){return texture.sample(uv);};
    }

    void bindTexture(Texture& tex) {
        texture = tex;
        value_ = [&](Vector2& uv){return texture.sample(uv);};
    }

    void bindTexture(Texture& tex, Vector3 scale) {
        texture = tex;
        this->scale = scale;
        value_ = [&](Vector2& uv){return this->scale*texture.sample(uv);};
    }

    T value(Vector2& uv) {
        return value_(uv);
    }

    T value() {
        return uniformValue;
    }

private:
    Texture texture;
    std::function<T(Vector2& uv)> value_;
    T uniformValue;
    T scale;
    
};

class Material {
public:

    Material();
    Material(Vector3 albedo, Vector3 emission);
    MaterialProperty<Vector3> albedo;
    MaterialProperty<Vector3> emission;
    virtual Vector3 getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector2 uv) = 0;
    virtual SampleBSDF sample(Vector3 dir, Vector3 n, Sampler* sampler) = 0;
    virtual double getPdf(Vector3 dir, Vector3 odir, Vector3 n) = 0;

};
