#include "ggx.h"

GGXMaterial::GGXMaterial() {
    roughness = 0.5;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}
GGXMaterial::GGXMaterial(Vector3 albedo) {
    this->albedo = albedo;
    specularAlbedo = Vector3(0.3,0.3,0.3);
    this->roughness = 0.5;
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
}

GGXMaterial::GGXMaterial(Vector3 albedo, Vector3 emission) {
    this->albedo = albedo;
    this->emission = emission;
}

GGXMaterial::GGXMaterial(Vector3 albedo, double roughness) : roughness(roughness) {
    this->albedo = albedo;
    specularAlbedo = Vector3(0.3,0.3,0.3);
    alpha = roughness * roughness;
    alpha2 = alpha*alpha;
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

    Vector3 diffuse = (albedo*IPI)*(Vector3(1,1,1)-specularAlbedo)*(28/23)*(1-pow(1-costi/2, 5))*(1-pow(1-costr/2, 5));
    
    return (specularAlbedo*d*go*gi*f + diffuse)*costi;
}

Sample3D GGXMaterial::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {

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

