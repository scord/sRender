#include "sampler.h"
#include <random>
#include <algorithm> 




Sampler::Sampler(int nSamples) : nSamples(nSamples) {

    double grid_size = sqrt(nSamples);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0,1);

    // stratified sampling
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            vectorSamples.push_back(Sample2D(Vector2(i/grid_size + dis(gen)/grid_size, j/grid_size + dis(gen)/grid_size), 1));
        }
    }

    sampleNumber = 0;
}

Sample2D Sampler::getSample() {
    return vectorSamples[sampleNumber++];
}

void Sampler::generateTestImage(const int width, const int height) {
    std::vector< std::vector<int> > pix;

    for (int i=0; i < width; i++) {
        pix.push_back(std::vector<int>());
        for (int j = 0; j < height; j++) {
            pix[i].push_back(0);
        }
    }

    for (int n=0; n < nSamples; n++) {
        Sample2D pixel = getSample();

        int xindex = (int)(width*(pixel.value.x+0.5));
        int yindex = (int)(height*(pixel.value.y+0.5));
        fprintf(stdout," %f ", pixel.value.x);
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

