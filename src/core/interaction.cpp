#include "interaction.h"


Interaction::Interaction(Vector3 direction, Vector3 pos, Vector3 n, Material* mat, bool backward) 
    : position(pos), normal(n), material(mat), backward(backward) {
    cost = direction.dot(n);
    if (backward) {
        odir = direction;
    } else {
        idir = direction;
    }
}

Interaction::Interaction(Vector3 odir, Vector3 idir, Vector3 pos, Vector3 n, Material* mat,  bool backward)
    : odir(odir), idir(idir), position(pos), normal(n), material(mat), backward(backward) {
    cost = odir.dot(n);
}

Sample3D Interaction::sample(Sampler* sampler) {
    if (backward) {
        Sample3D sample = material->sample(odir, idir, normal, sampler);
        idir = sample.value;
        return sample;
    } else {
        Sample3D sample = material->sample(idir, odir, normal, sampler);
        odir = sample.value;
        return sample;
    }
}

Sample3D Interaction::sample(Sampler* sampler, Vector3& reflectance) {
    if (backward) {
        Sample3D sample = material->sample(odir, idir, normal, reflectance, sampler);
        idir = sample.value;
        return sample;
    } else {
        Sample3D sample = material->sample(idir, odir, normal, reflectance, sampler);
        odir = sample.value;
        return sample;
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

Vector3 Interaction::getReflectance() {
    if (backward) {
        return material->reflectance(idir, odir, normal);
    } else {
        return material->reflectance(odir, idir, normal);
    }
}





