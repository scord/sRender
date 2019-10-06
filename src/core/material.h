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
        value_ = [](vec2& uv){return T();};
    }

    MaterialProperty(T value) {
        uniformValue = value;
        value_ = [&](vec2& uv){return uniformValue;};
    }

    MaterialProperty(Texture& tex) {
        texture = tex;
        value_ = [&](vec2& uv){return texture.sample(uv);};
    }

    void bindTexture(Texture& tex) {
        texture = tex;
        value_ = [&](vec2& uv){return texture.sample(uv);};
    }

    void bindTexture(Texture& tex, vec3 scale) {
        texture = tex;
        this->scale = scale;
        value_ = [&](vec2& uv){return this->scale*texture.sample(uv);};
    }

    T value(vec2& uv) {
        return value_(uv);
    }

    T value() {
        return uniformValue;
    }

private:
    Texture texture;
    std::function<T(vec2& uv)> value_;
    T uniformValue;
    T scale;
    
};

class Material {
public:

    Material();
    Material(vec3 albedo, vec3 emission);
    MaterialProperty<vec3> albedo;
    MaterialProperty<vec3> emission;
    virtual vec3 getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv) = 0;
    virtual SampleBSDF sample(vec3 dir, vec3 n, Sampler& sampler) = 0;
    virtual double getPdf(vec3 dir, vec3 odir, vec3 n) = 0;

};
