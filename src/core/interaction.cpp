#include "interaction.h"


Interaction::Interaction(Vector3 direction, Vector3 pos, Vector2 uv, Vector3 n, Material* mat, bool backward, double t) 
    : position(pos), uv(uv), normal(n), material(mat), backward(backward), t(t) {
    
    odir = direction;
    emission = material->emission.value(uv);
}

Interaction::Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector2 uv, Vector3 n, Material* mat,  bool backward, double pdf)
    : odir(odir), idir(idir), position(pos), uv(uv), normal(n), material(mat), backward(backward), pdf(pdf) {
    cost = idir.dot(n);
    brdf = getBrdf();
    emission = material->emission.value(uv);
}

Interaction::Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector2 uv, Vector3 n, Material* mat,  bool backward, double pdf, double t)
    : Interaction(odir, idir, pos, uv, n, mat, backward, pdf) {

    this->t = t;

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
    return material->getBrdf(idir, odir, normal, uv);
}







