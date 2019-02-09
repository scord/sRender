#include "interaction.h"


Interaction::Interaction(Vector3 direction, Vector3 pos, Vector3 n, Material* mat, bool backward) 
    : position(pos), normal(n), material(mat), backward(backward) {
    if (backward) {
        odir = direction;
    } else {
        idir = direction;
    }
}

Interaction::Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector3 n, Material* mat,  bool backward)
    : odir(odir), idir(idir), position(pos), normal(n), material(mat), backward(backward) {

}

void Interaction::sample(Sampler* sampler) {
    if (backward) {
        idir = material->sample(odir, idir, normal, sampler).value;
    } else {
        odir = material->sample(idir, odir, normal, sampler).value;
    }
}

Ray Interaction::getOutgoing() {
    return Ray(position, odir);
}

Ray Interaction::getIncoming() {
    return Ray(position, idir);
}

double Interaction::getPdf() {
    if (backward) {
        return material->getPdf(idir, odir, normal);
    }  else {
        return material->getPdf(odir, idir, normal);
    }
}

Vector3 Interaction::getBrdf() {
    if (backward) {
        return material->getBrdf(idir, odir, normal);
    } else {
        return material->getBrdf(odir, idir, normal);
    }
}





