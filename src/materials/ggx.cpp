#include "ggx.h"

GGXMaterial::GGXMaterial() : hemisphere(Vector3(), 1) {
    roughness = 0.4;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}
GGXMaterial::GGXMaterial(Vector3 albedo) : hemisphere(Vector3(), 1) {
    this->albedo = albedo;
    roughness = 0.4;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

GGXMaterial::GGXMaterial(Vector3 albedo, Vector3 emission) : hemisphere(Vector3(), 1) {
    this->albedo = albedo;
    this->emission = emission;
}

GGXMaterial::GGXMaterial(Vector3 albedo, double roughness) : hemisphere(Vector3(), 1), roughness(roughness) {
    this->albedo = albedo;
}

double GGXMaterial::distribution(double cost) {
    double cos2t = cost*cost;
    return alpha2/(PI*std::pow(cos2t*(alpha2 - 1) + 1,2));
}

double GGXMaterial::masking(double cost) {
    return 1/(cost + std::sqrt(alpha2 + (1-alpha2)*(cost*cost)));
}

double GGXMaterial::fresnel(double etai, double etat, double cost) {
    double f0 = std::pow((etai - etat)/(etai+etat), 2);
    return f0 + (1-f0)*(1-std::pow(1-cost, 5));
}

Vector3 GGXMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {
    double costi = n.dot(dir);
    if (costi < 0) {
        return Vector3();
    }

    double etai = 1;
    double etat = 1.5;

    Vector3 wh = (dir+odir).norm();

    double cost = n.dot(wh);
    double costr = n.dot(odir);

    double d = distribution(cost);
    double go = masking(costr);
    double gi = masking(costi);
    double f = fresnel(etai, etat, odir.dot(wh));
    
    return albedo*d*go*gi*f*costi;
}

Sample3D GGXMaterial::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {
    Sample3D sample = hemisphere.map(sampler->getSample2D());
   // Vector3 test = sample.value.rotationMatrix()*n ;

    double r0 = sampler->getRandomDouble();
    double cost = std::sqrt((1 - r0)/((alpha2-1)*r0 + 1));
    double r1 = sampler->getRandomDouble();
    double phi = TAU*r1;
    double sint = std::sqrt(1 - cost*cost);
    double x = sint * std::cos(phi);
    double y = sint * std::sin(phi);
    double z = cost;
    Vector3 wm = Vector3(x,y,z).norm().rotate(n);
    Vector3 wi = ((Vector3() - idir) - wm*((Vector3() - idir).dot(wm)*2)).norm();

    double exp = (alpha2-1)*cost*cost + 1;
    double d = alpha2 / (PI * exp * exp);
    double pdf = (d * wm.dot(n)) / (4* wi.dot(wm));
    return Sample3D(wi, pdf);
}

double GGXMaterial::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {

    Vector3 wm = (idir + odir).norm();

    double cost = n.dot(wm);
    double exp = (alpha2-1)*cost*cost + 1;
    double d = alpha2 / (PI * exp * exp);
    double pdf = (d * wm.dot(n)) / (4* idir.dot(wm));

    return pdf;
}

