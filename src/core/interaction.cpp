#include "interaction.h"

MediumInteraction::MediumInteraction(vec3 pos, Medium* medium) : position(pos), medium(medium) {};

Interaction::Interaction(vec3 direction, vec3 pos, vec2 uv, vec3 n, Material* mat, bool backward) 
    : position(pos), uv(uv), normal(n), material(mat), backward(backward) {
    hit =true;
    odir = direction;
    emission = material->emission.value(uv);
}

Interaction::Interaction(vec3 dir, vec3 pos) : position(pos) {
    hit = false; 
    odir = dir;
}

Interaction::Interaction()  {
    hit = false; 
}

Interaction::Interaction(vec3 odir, vec3 idir, vec3 pos, vec2 uv, vec3 n, Material* mat,  bool backward, double pdf)
    : odir(odir), idir(idir), position(pos), uv(uv), normal(n), material(mat), backward(backward), pdf(pdf) {
    hit = true;
    cost = idir.dot(n);
    brdf = getBrdf();
    emission = material->emission.value(uv);
}

void Interaction::forceSample(vec3 idir, double pdf) {
    this->idir = idir;
    cost = idir.dot(normal);
    this->pdf = pdf;
    brdf = getBrdf();
}

void Interaction::sample(Sampler& sampler) {
    SampleBSDF sample = material->sample(odir, normal, sampler);
    cost = sample.cost;
    pdf = sample.pdf;
    idir = sample.value;
    brdf = getBrdf();
}

Ray Interaction::getOutgoing() {
    return Ray(position, odir);
}

Ray Interaction::getIncoming() {
    return Ray(position, idir);
}

double Interaction::getPdf() {
    return material->getPdf(idir, odir, normal);
}

vec3 Interaction::getBrdf() {
    return material->getBrdf(idir, odir, normal, uv);
}







