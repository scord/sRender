#include "blendmaterial.h"


BlendMaterial::BlendMaterial(Material* mat1, Material* mat2, double blend) : mat1(mat1), mat2(mat2), blend(blend) {

}

Vector3 BlendMaterial::getBrdf(Vector3 dir, Vector3 odir, Vector3 n) {
    return mat1->getBrdf(dir, odir, n)*blend + mat2->getBrdf(dir, odir, n)*(1-blend);
}

Sample3D BlendMaterial::sample(Vector3 idir, Vector3 odir, Vector3 n, Sampler* sampler) {
    double b = sampler->getRandomDouble();

    if (b < blend)
        return mat1->sample(idir, odir, n, sampler);
    else
        return mat2->sample(idir, odir, n, sampler);
    
}

double BlendMaterial::getPdf(Vector3 idir, Vector3 odir, Vector3 n) {
   return mat1->getPdf(idir, odir, n)*blend + mat2->getPdf(idir, odir, n)*(1-blend);
}

