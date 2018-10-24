#include "core/smath.h"
#include "core/camera.h"
#include <stdio.h>
#include "samplers/pixelsampler.h"
#include <algorithm>
#include <vector>
#include "core/geometry.h"
#include "core/integrator.h"
#include "brdfs/diffuse.h"
#include "brdfs/mirror.h"

#define PI 3.1415926536

int main() {

	Camera cam(Vector3(0,0,0.9), 100, 100, PI/4);

	std::vector<Shape*> scene;

	// ROOM
	scene.push_back(new Plane(Vector3(0,0,1), Vector3(0,0,-1), Vector3(1,1,1), new DiffuseBrdf()));
	scene.push_back(new Plane(Vector3(0,1,0), Vector3(0,-1,0), Vector3(1,1,1), new DiffuseBrdf()));
	scene.push_back(new Plane(Vector3(0,-1,0), Vector3(0,1,0), Vector3(1,1,1), new DiffuseBrdf()));
	scene.push_back(new Plane(Vector3(0,0,-1), Vector3(0,0,1), Vector3(1,1,1), new DiffuseBrdf()));
	scene.push_back(new Plane(Vector3(1,0,0), Vector3(-1,0,0), Vector3(1,0,0), new DiffuseBrdf()));
	scene.push_back(new Plane(Vector3(-1,0,0), Vector3(1,0,0), Vector3(0,1,0), new DiffuseBrdf()));
	
	// BOX
	scene.push_back(new Quad(Vector3(-0.5,0.1,-0.5), Vector3(0,1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.25), Vector3(0,0,-1)));
	scene.push_back(new Quad(Vector3(-0.75,-0.45,-0.5), Vector3(-1,0,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(-0.25,-0.45,-0.5), Vector3(1,0,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.25), Vector3(0,0,1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.75), Vector3(0,0,-1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));

	// SPHERE
	scene.push_back(new Sphere(Vector3(0.5, -0.7, -0.55), Vector3(1,1,1), new MirrorBrdf(), 0.3));
	
	// LIGHT
	Disc* areaLight = new Disc(Vector3(0,0.999,0), Vector3(0,-1,0), Vector3(1000,1000,1000), new DiffuseBrdf(), Vector2(0.5,0.5));
	areaLight->isLight = true;
	
	scene.push_back(areaLight);
	
	std::vector<std::vector<Vector3>> image;

	for (int i=0; i < cam.width; i++) {
        image.push_back(std::vector<Vector3>());
        for (int j = 0; j < cam.height; j++) {
            image[i].push_back(Vector3(0,0,0));
        }
    }

	Integrator pathTracer;

	int samplesPerPixel = 128;
	std::vector<std::vector<PixelSampler>> pixelSamplers;

	// prepare samplers
	for (int i=0; i < cam.width; i++) {
		pixelSamplers.push_back(std::vector<PixelSampler>());
		for (int j=0; j < cam.height; j++) {
			PixelSampler pixelSampler(samplesPerPixel);
			pixelSamplers[i].push_back(pixelSampler);
		}
	}

	for (int n=0; n < samplesPerPixel; n++) {
		for (int i=0; i < cam.width; i++) {
			for (int j=0; j < cam.height; j++) {
				
				Ray ray = cam.pixelToRay(pixelSamplers[i][j].getPixelSample(Vector2(i+0.5, j+0.5)));
				Vector3 colour = pathTracer.getRadiance(ray, 0, scene);
				image[i][j] += colour.clamp()/samplesPerPixel;
			}
		}

		fprintf(stdout, "\r%d samples ", n+1);
		fflush(stdout);
	}

	FILE *f = fopen("planetest.bmp", "w");
	fprintf(f, "P3\n%d %d\n%d\n ",cam.width,cam.height,255);
	for (int j=0;j<cam.width;j++) {
		for (int i=0;i<cam.height;i++) {
			fprintf(f,"%d %d %d ", (int)(255*image[i][j].x), 
								   (int)(255*image[i][j].y), 
								   (int)(255*image[i][j].z));
		}
		fprintf(f, "\n");
	}
	fclose(f);

	//PixelSampler sampler2(4096);
	//sampler2.GenerateTestImage(200, 200);

	return 0;
}
