#include "brdf.h"

Brdf::Brdf() {}

void Brdf::giveSample(Sample3D sample) {
    this->sample = sample;
}