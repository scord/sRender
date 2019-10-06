#pragma once

#include "../core/smath.h"
#include <random>
#include "../core/sample.h"
#include "../samplers/sampler.h"
#include <functional>
#include "../core/texture.h"
#include "geometry.h"
#include "texture.h"


class Medium {
public:

    Medium();
    Medium(vec3 absorption, vec3 scattering, vec3 emission, double density);

    vec3 absorption;
    vec3 scattering;
    vec3 emission;
    vec3 attenuation;
    
    

    double density;


    vec3 transmittance(double distance) {
        if (density <= 0.0)
            return vec3(1,1,1);
        
        vec3 tau = (absorption + scattering) * distance / density;
  
        return vec3(exp(-tau.x), exp(-tau.y), exp(-tau.z));
    }

};
