#include "fresnelblend.h"
#include "../core/smath.h"
#include <iostream>
#include <assert.h>
FresnelBlend::FresnelBlend() : hemisphere(Hemisphere(Vector3(), 1)){
    roughness = 0.5;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}
FresnelBlend::FresnelBlend(Vector3 albedo) : hemisphere(Hemisphere(Vector3(), 1)){
    this->albedo = albedo;
    this->roughness = 1.0;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

FresnelBlend::FresnelBlend(Vector3 albedo, Vector3 emission) : hemisphere(Hemisphere(Vector3(), 1)){
    this->albedo = albedo;
    this->emission = emission;
}

FresnelBlend::FresnelBlend(Vector3 albedo, double roughness) : roughness(roughness), hemisphere(Hemisphere(Vector3(), 1)) {
    this->albedo = albedo;
    this->specularAlbedo = albedo;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

FresnelBlend::FresnelBlend(Vector3 albedo, Vector3 specularAlbedo, double roughness) : roughness(roughness), hemisphere(Hemisphere(Vector3(), 1)) {
    this->albedo = albedo;
    this->specularAlbedo = specularAlbedo;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

double FresnelBlend::distribution(double cost) {
    double cos2t = cost*cost;
    return alpha2/(PI*std::pow(cos2t*(alpha2 - 1) + 1,2));
}

// masking
double FresnelBlend::g1(double cost) {
    return 2.0*cost/(cost + std::sqrt(alpha2 + (1-alpha2)*(cost*cost)));
}

double FresnelBlend::g1DividedBy2Cos(double cost) {
    return 1.0/(cost + std::sqrt(alpha2 + (1-alpha2)*(cost*cost)));
}

// masking shadowing
double FresnelBlend::g2(double costi, double costo) {
    return 2.0*costi*costo/(costo*std::sqrt(alpha2+(1-alpha2)*costi*costi) + costi*std::sqrt(alpha2+(1-alpha2)*costo*costo) );
}

double FresnelBlend::g2DividedBy2CosiCoso(double costi, double costo) {
    return 1.0/(costo*std::sqrt(alpha2+(1-alpha2)*costi*costi) + costi*std::sqrt(alpha2+(1-alpha2)*costo*costo) );
}



double FresnelBlend::fresnel(double etai, double etat, double cost) {
    double f0 = std::pow((etai - etat)/(etai+etat), 2);
    return f0 + (1-f0)*std::pow(1-cost, 5);
}

Vector3 FresnelBlend::fresnel(Vector3 r0, double cost) {
    return r0 + (Vector3(1,1,1) - r0)*std::pow(1-cost, 5);
}

Vector3 FresnelBlend::getBrdf(Vector3 idir, Vector3 odir, Vector3 n) {
    double costi = n.dot(idir);
    if (costi < 0) {
        return Vector3();
    }


    Vector3 wm = (idir+odir).norm();

    double cost = n.dot(wm);
    double costo = n.dot(odir);

    double d = distribution(cost);

    double g = g2(costi, costo);
    Vector3 f = fresnel(specularAlbedo, idir.dot(wm));

    Vector3 diffuse = (albedo*IPI)*(Vector3(1,1,1)-specularAlbedo)*(28.0/23.0)*(1-std::pow(1-costi/2, 5))*(1-std::pow(1-costo/2, 5));
    
    return (diffuse + specularAlbedo*d*g*f/(4*costi*costo));
}

Vector3 FresnelBlend::getBrdf(double nidir, double nodir, double mdir, double d) {
    if (nidir < 0)
        return Vector3();
    
    Vector3 f = fresnel(specularAlbedo, mdir);
    double g = g2DividedBy2CosiCoso(nidir, nodir);
    Vector3 diffuse = (albedo*IPI)*(Vector3(1,1,1)-specularAlbedo)*(28.0/23.0)*(1-std::pow(1-nidir/2, 5))*(1-std::pow(1-nodir/2, 5));
    return diffuse + specularAlbedo*d*g*f/2.0;
}

SampleBSDF FresnelBlend::sampleSpecular(Vector3 odir, Vector3 n, Sampler* sampler) {
    Vector3 dirPerp = (odir - n*odir.dot(n));
    Vector3 t1 = dirPerp.norm().cross(n).norm();
    Vector3 stretchedDir = (dirPerp*alpha + n*odir.dot(n)).norm();
    Vector3 t2 = t1.cross(stretchedDir).norm();

    double u1 = sampler->getRandomDouble();
    double u2 = sampler->getRandomDouble();
   
    double a2 = 1;
    double a1 = (1 - std::abs(n.dot(stretchedDir))); 

    double a = 1/(1+a1);

    double r = std::sqrt(u1);
    double theta = 0;

    if (u2 < a) {
        theta = (u2 / a) * PI  ;
    } else {
        theta = PI + (u2 - a) / (1-a) * PI ;
        t2 = dirPerp;
    }

    double p1 = r*std::cos(theta);
    double p2 = r*std::sin(theta);
    Vector3 t = t2*p2 + t1*p1;
    double distToEdge = std::sqrt(1-p1*p1-p2*p2);
    Vector3 mn = t+stretchedDir*distToEdge;
    double mndotn = mn.dot(n);
    dirPerp = mn - n*mndotn;
    Vector3 wm = (dirPerp*alpha + n*mndotn).norm();

    double dirdotwm = odir.dot(wm);
    Vector3 idir = wm*dirdotwm*2 - odir;
    double costi = n.dot(idir);
    double costo = n.dot(odir);
    double costm = n.dot(wm);
    double d = distribution(costm);
    if (costi < 0)
        return SampleBSDF(idir, Vector3(), 0, getPdf(idir, odir, n));

    return SampleBSDF(idir, getBrdf(costi, costo, dirdotwm, d), costi, getPdf(idir, odir, n));
}

SampleBSDF FresnelBlend::sampleDiffuse(Vector3 odir, Vector3 n, Sampler* sampler) {
    Sample3D sample = hemisphere.map(sampler->getSample2D());
    double costi = sample.value.z;
    Vector3 idir = sample.value.rotate(n);

    Vector3 wm = (idir+odir).norm();
    double costm = n.dot(wm);
    double d = distribution(costm);

    return SampleBSDF(idir, getBrdf(costi, n.dot(odir), odir.dot(wm), d), costi, getPdf(idir, odir, n));
}

SampleBSDF FresnelBlend::sample(Vector3 odir, Vector3 n, Sampler* sampler) {
    
    double diffuseSwitch = sampler->getRandomDouble();

    if (diffuseSwitch > 0.5) {
        sampler->recycledDouble = (diffuseSwitch - 0.5) / 0.5;
        return sampleDiffuse(odir, n, sampler);
    } else {
        sampler->recycledDouble = diffuseSwitch/0.5;
        return sampleSpecular(odir, n, sampler);
    }
}



double FresnelBlend::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {

    Vector3 wm = (idir + odir).norm();
    double d = distribution(wm.dot(n));
    double pdf = g1DividedBy2Cos(odir.dot(n))*d /2.0;

    return 0.5*pdf + 0.5*std::abs(idir.dot(n))*IPI;
}

