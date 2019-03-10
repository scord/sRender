#include "interaction.h"


Interaction::Interaction(Vector3 direction, Vector3 pos, Vector3 n, Material* mat, bool backward) 
    : position(pos), normal(n), material(mat), backward(backward) {
    
    odir = direction;

}

Interaction::Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector3 n, Material* mat,  bool backward, double pdf)
    : odir(odir), idir(idir), position(pos), normal(n), material(mat), backward(backward), pdf(pdf) {
    cost = idir.dot(n);
    brdf = getBrdf();
}

void Interaction::forceSample(Vector3 idir, double pdf) {
    this->idir = idir;
    cost = idir.dot(normal);
    this->pdf = pdf;
    brdf = getBrdf();
}

void Interaction::sample(Sampler* sampler) {
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

Vector3 Interaction::getBrdf() {
    return material->getBrdf(idir, odir, normal, position);
}

Vector3 Interaction::getReflectance() {
    return material->reflectance(idir, odir, normal);
}





