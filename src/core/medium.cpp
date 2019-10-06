#include "medium.h"

Medium::Medium() : Medium(vec3(0.1,0.1,0.1), vec3(0.2,0.2,0.2), vec3(0.1,0.1,0.1), 1.0) {};

Medium::Medium(vec3 absorption, vec3 scattering, vec3 emission, double density) {
    this->absorption = absorption*density;
    this->scattering = scattering*density;
    this->emission = emission*density;
    this->density = density;
    this->attenuation = this->absorption + this->scattering;
}
