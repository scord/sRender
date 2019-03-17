#include "emissive.h"

EmissiveMaterial::EmissiveMaterial(){
}
EmissiveMaterial::EmissiveMaterial(Vector3 emission) : Material(Vector3(), emission) {
}

EmissiveMaterial::EmissiveMaterial(Texture texture, Vector3 emission) : Material(Vector3(), emission), texture(texture) {
    this->emission.bindTexture(texture, emission);
}



Vector3 EmissiveMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n, Vector2 uv) {
    return Vector3();
}



SampleBSDF EmissiveMaterial::sample(Vector3 odir,  Vector3 n, Sampler* sampler) {
    return SampleBSDF(Vector3(), Vector3(), 0, 1);
}

double EmissiveMaterial::getPdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return 1;
}
