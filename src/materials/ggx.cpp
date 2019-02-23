#include "ggx.h"
#include "../core/smath.h"

GGXMaterial::GGXMaterial() {
    roughness = 0.5;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}
GGXMaterial::GGXMaterial(Vector3 albedo) {
    this->albedo = albedo;
    this->roughness = 1.0;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

GGXMaterial::GGXMaterial(Vector3 albedo, Vector3 emission) {
    this->albedo = albedo;
    this->emission = emission;
}

GGXMaterial::GGXMaterial(Vector3 albedo, double roughness) : roughness(roughness) {
    this->albedo = albedo;
    this->specularAlbedo = albedo;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

GGXMaterial::GGXMaterial(Vector3 albedo, Vector3 specularAlbedo, double roughness) : roughness(roughness) {
    this->albedo = albedo;
    this->specularAlbedo = specularAlbedo;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

double GGXMaterial::distribution(double cost) {
    double cos2t = cost*cost;
    return alpha2/(PI*std::pow(cos2t*(alpha2 - 1) + 1,2));
}

double GGXMaterial::masking(double cost) {
    return (2.0*cost)/(cost + std::sqrt(alpha2 + (1-alpha2)*(cost*cost)));
}

double GGXMaterial::g2(double costi, double costr) {
    return 2.0*costi*costr/(costr*std::sqrt(alpha2+(1-alpha2)*costi*costi) + costi*std::sqrt(alpha2+(1-alpha2)*costr*costr) );
}

double GGXMaterial::fresnel(double etai, double etat, double cost) {
    double f0 = std::pow((etai - etat)/(etai+etat), 2);
    return f0 + (1-f0)*(std::pow(1-cost, 5));
}

Vector3 GGXMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {
    double costi = n.dot(dir);
    if (costi < 0.0) {
        return Vector3();
    }

    double etai = 1.0;
    double etat = 20.0;

    Vector3 wh = (dir+odir).norm();

    double cost = n.dot(wh);
    double costr = n.dot(odir);

    double d = distribution(cost);
    double g = g2(costi, costr);
    double f = fresnel(etai, etat, dir.dot(wh));

    return (specularAlbedo*d*f*g2(costi,costr)/(4*costi*costr))*costi;
}
/*
Sample3D GGXMaterial::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {
    double r0 = sampler->getRandomDouble();
    double cost = std::sqrt((1 - r0)/((alpha2-1)*r0 + 1));
    double r1 = sampler->getRandomDouble();
    double phi = TAU*r1;
    double sint = std::sqrt(1 - cost*cost);
    double x = sint * std::sin(phi);
    double y = sint * std::cos(phi);
    double z = cost;
    Vector3 wm = Vector3(x,y,z).rotate(n);
    double len = wm.length();
    
    Vector3 wi = ((Vector3() - idir) - wm*((Vector3() - idir).dot(wm)*2));

    double d = distribution(cost);
    double pdf = (d * cost) / (4 * wi.dot(wm));
    return Sample3D(wi, pdf);
}
*/

Sample3D GGXMaterial::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {

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
        length = std::sqrt(std::max(0.0,1.0-p1*p1-p2*p2));
    } else {
        double theta = (u2 - a) / (1-a) * PI ;
        double p1 = r*std::cos(theta);
        double p2 = r*std::sin(theta);
        t = dirPerp*p2 + t1*p1;
        length = std::sqrt(std::max(0.0,1.0-p1*p1-p2*p2));
    }

    Vector3 mn = (t+stretchedDir*length);
    dirPerp = mn - n*mn.dot(n);
    Vector3 wm = (dirPerp*alpha + n*mn.dot(n)).norm();

    Vector3 wi = ((Vector3() - idir) - wm*((Vector3() - idir).dot(wm)*2));

    double d = distribution(wm.dot(n));
    double pdf = masking(idir.dot(n))*d / (idir.dot(n)*4);
    return Sample3D(wi, pdf);
}

Vector3 GGXMaterial::reflectance(Vector3 idir, Vector3 odir, Vector3 n) {
    double costi = n.dot(idir);
   
    if (costi < 0) {
        return Vector3();
    }

    double etai = 1.0;
    double etat = 1.5;

    Vector3 wh = (idir+odir).norm();

    double costh = odir.dot(wh);
    double costr = n.dot(odir);

    double g = g2(costi, costr);
    double f = fresnel(etai, etat, costh);

    return specularAlbedo*g*f/masking(costh);
}

double GGXMaterial::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {
    Vector3 wm = (idir + odir).norm();
    double d = distribution(wm.dot(n));
    return masking(odir.dot(n))*d / (odir.dot(n)*4);
}


