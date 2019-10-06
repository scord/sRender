#include "emissive.h"

EmissiveMaterial::EmissiveMaterial(){
}
EmissiveMaterial::EmissiveMaterial(vec3 emission) : Material(vec3(), emission) {
}

EmissiveMaterial::EmissiveMaterial(Texture texture, vec3 emission) : Material(vec3(), emission), texture(texture) {
    this->emission.bindTexture(texture, emission);
}



vec3 EmissiveMaterial::getBrdf(vec3 dir, vec3 odir, vec3 n, vec2 uv) {
    return vec3();
}



SampleBSDF EmissiveMaterial::sample(vec3 odir,  vec3 n, Sampler& sampler) {
    return SampleBSDF(vec3(), vec3(), 0, 1);
}

double EmissiveMaterial::getPdf(vec3 dir, vec3 odir, vec3 n) {
    return 1;
}
