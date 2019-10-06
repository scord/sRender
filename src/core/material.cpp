#include "material.h"

Material::Material() : albedo(vec3(0.9,0.9,0.9)), emission(vec3(0,0,0)) {}

Material::Material(vec3 albedo, vec3 emission) : albedo(albedo), emission(emission) {}