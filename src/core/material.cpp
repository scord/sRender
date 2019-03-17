#include "material.h"

Material::Material() : albedo(Vector3(0.9,0.9,0.9)), emission(Vector3(0,0,0)) {}

Material::Material(Vector3 albedo, Vector3 emission) : albedo(albedo), emission(emission) {}