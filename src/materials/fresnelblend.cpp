#include "fresnelblend.h"
#include "../core/smath.h"

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
/*
double FresnelBlend::g1DividedBy2Cos(double cost) {
    return 1.0/(cost + std::sqrt(alpha2 + (1-alpha2)*(cost*cost)));
}*/

// masking shadowing
double FresnelBlend::g2(double costi, double costo) {
    return 2.0*costi*costo/(costo*std::sqrt(alpha2+(1-alpha2)*costi*costi) + costi*std::sqrt(alpha2+(1-alpha2)*costo*costo) );
}
/*
double FresnelBlend::g2DividedBy2CosiCoso(double costi, double costo) {
    return 1.0/(costo*std::sqrt(alpha2+(1-alpha2)*costi*costi) + costi*std::sqrt(alpha2+(1-alpha2)*costo*costo) );
}*/



double FresnelBlend::fresnel(double etai, double etat, double cost) {
    double f0 = std::pow((etai - etat)/(etai+etat), 2);
    return f0 + (1-f0)*std::pow(1-cost, 5);
}

Vector3 FresnelBlend::fresnel(Vector3 r0, double cost) {
    return r0 + (Vector3(1,1,1) - r0)*std::pow(1-cost, 5);
}

Vector3 FresnelBlend::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {
    double costi = n.dot(dir);
    if (costi < 0) {
        return Vector3();
    }

    Vector3 wh = (dir+odir).norm();

    double cost = n.dot(wh);
    double costo = n.dot(odir);

    double d = distribution(cost);

    double g = g2(costi, costo);
    double c = odir.dot(wh);
    Vector3 f = fresnel(specularAlbedo, dir.dot(wh));

    Vector3 diffuse = (albedo*IPI)*(Vector3(1,1,1)-specularAlbedo)*(28.0/23.0)*(1-std::pow(1-costi/2, 5))*(1-std::pow(1-costo/2, 5));
    
    return (diffuse + specularAlbedo*d*g*f/(4*costi*costo))*costi;
}

Sample3D FresnelBlend::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {

    double diffuseSwitch = sampler->getRandomDouble();

    if (diffuseSwitch > 0.5) {
        Sample3D sample = hemisphere.map(sampler->getSample2D());
        return Sample3D(sample.value.rotate(n), sample.pdf);
    }

    Vector3 dirPerp = (idir - n*idir.dot(n));
    Vector3 t1 = dirPerp.norm().cross(n).norm();
    Vector3 stretchedDir = (dirPerp*alpha + n*idir.dot(n)).norm();
    Vector3 t2 = t1.cross(stretchedDir).norm();

    double u1 = sampler->getRandomDouble();
    double u2 = sampler->getRandomDouble();
   
    double a2 = 1;
    double a1 =(1- std::abs(n.dot(stretchedDir))); 

    double a = 1/(1+a1);

    double r = std::sqrt(u1);
    
    Vector3 t(0,0,0);
    double length = 1;
    if (u2 < a) {
        double theta = (u2 / a) * PI  ;
        double p1 = r*std::cos(theta);
        double p2 = r*std::sin(theta);
        t = t2*p2 + t1*p1;
        length = std::sqrt(1-p1*p1-p2*p2);
    } else {
        double theta = PI + (u2 - a) / (1-a) * PI ;
        double p1 = r*std::cos(theta);
        double p2 = r*std::sin(theta);
        t = dirPerp*p2 + t1*p1;
        length = std::sqrt(1-p1*p1-p2*p2);
    }

    Vector3 mn = (t+stretchedDir*length).norm();
    dirPerp = mn - n*mn.dot(n);
    Vector3 wm = (dirPerp*alpha + n*mn.dot(n)).norm();

    Vector3 wi = ((Vector3() - idir) - wm*((Vector3() - idir).dot(wm)*2));

    double d = distribution(wm.dot(n));
    double pdf = g1(idir.dot(n))*d / (idir.dot(n)*4);
    return Sample3D(wi, pdf);
}

double FresnelBlend::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {

    Vector3 wm = (idir + odir).norm();
    double d = distribution(wm.dot(n));
    double pdf = g1(odir.dot(n))*d / (odir.dot(n)*4);

    return 0.5*pdf + 0.5*std::abs(idir.dot(n))*IPI;
}

