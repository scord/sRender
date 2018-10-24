#include "pixelsampler.h"
#include <random>


PixelSampler::PixelSampler(int samplesPerPixel) : Sampler(samplesPerPixel) {
    std::random_device rd;
    std::mt19937 gen(rd());
    double grid_size = sqrt(samplesPerPixel);
    
    std::uniform_real_distribution<double> dis(0, 1);

    // stratified sampling
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            doubleSamples.push_back(dis(gen));
            vectorSamples.push_back(Vector2(i/grid_size + 1/(2*grid_size) + (dis(gen)-0.5)/grid_size - 0.5, j/grid_size + 1/(2*grid_size) + (dis(gen)-0.5)/grid_size -0.5));
        }
    }
    
    sampleNumber = 0;
}

double PixelSampler::getdoubleSample() {
    return doubleSamples[sampleNumber++];
}

Vector2 PixelSampler::getVectorSample() {
    return vectorSamples[sampleNumber++];
}

Vector2 PixelSampler::getPixelSample(const Vector2 &p) {
    return p + getVectorSample();
}

void PixelSampler::generateTestImage(const int width, const int height) {
    std::vector< std::vector<int> > pix;

    for (int i=0; i < width; i++) {
        pix.push_back(std::vector<int>());
        for (int j = 0; j < height; j++) {
            pix[i].push_back(0);
        }
    }

    for (int n=0; n < samplesPerPixel; n++) {
        Vector2 pixel = getVectorSample();

        int xindex = (int)(width*(pixel.x+0.5));
        int yindex = (int)(height*(pixel.y+0.5));
        fprintf(stdout," %f ", pixel.x);
        pix[xindex][yindex] = 255;
    }
	

	FILE *f = fopen("samplertest.bmp", "w");
	fprintf(f, "P3\n%d %d\n%d\n ",width,height,255);
	for (int row=0;row<width;row++) {
		for (int col=0;col<height;col++) {
			fprintf(f,"%d %d %d ", (int)pix[col][row], (int)pix[col][row], (int)pix[col][row]);
		}
		fprintf(f, "\n");
	}
	fclose(f);

}

