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
#include <thread>
#include <future>
#include <chrono>
#include <iostream>
#include "core/tonemapper.h"

#define PI 3.1415926536

std::vector<std::vector<Vector3>> renderTile(int tileNumber,  std::vector<std::vector<Vector3>> tile, Camera cam, std::vector<Shape*> scene, int samplesPerPixel, std::vector<std::vector<PixelSampler>> pixelSamplers);

int main() {

	Camera cam(Vector3(0,0,0.9), 300, 300, PI/4);

	std::vector<Shape*> scene;

	// ROOM
	scene.push_back(new Quad(Vector3(0,0,1), Vector3(0,0,-1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,1,0)));;
	scene.push_back(new Quad(Vector3(0,1,0), Vector3(0,-1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));
	scene.push_back(new Quad(Vector3(0,-1,0), Vector3(0,1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));
	scene.push_back(new Quad(Vector3(0,0,-1), Vector3(0,0,1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(1,0,0), Vector3(-1,0,0), Vector3(1,0,0), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));
	scene.push_back(new Quad(Vector3(-1,0,0), Vector3(1,0,0), Vector3(0,1,0), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));

	// BOX
	scene.push_back(new Quad(Vector3(-0.5,0.1,-0.5), Vector3(0,1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.25), Vector3(0,0,-1)));
	scene.push_back(new Quad(Vector3(-0.75,-0.45,-0.5), Vector3(-1,0,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(-0.25,-0.45,-0.5), Vector3(1,0,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.25), Vector3(0,0,1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.75), Vector3(0,0,-1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));

	// SPHERE
	scene.push_back(new Sphere(Vector3(0.5, -0.7, -0.55), Vector3(1,1,1), new MirrorBrdf(), 0.3));

	// LIGHT
	Disc* areaLight = new Disc(Vector3(0,0.999,0), Vector3(0,-1,0), Vector3(10000,9800,9500), new DiffuseBrdf(), Vector2(0.25,0.25));
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

	int samplesPerPixel = 1024;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	// prepare tiles

	int tileCount = std::thread::hardware_concurrency();
	int tileWidth = cam.width/tileCount;
	std::vector<std::vector<std::vector<Vector3>>> tiles;

	for (int t = 0; t < tileCount; t++) {

		std::vector<std::vector<Vector3>> tile;
		
		for (int i = 0; i < tileWidth; i++) {
			tile.push_back(std::vector<Vector3>());
			for (int j =0; j < cam.height; j++) {
				tile[i].push_back(Vector3(0,0,0));
			}
		}
		tiles.push_back(tile);
	}


	std::vector<std::future<std::vector<std::vector<Vector3>>>> resultTiles;

	for (int t = 0; t < tileCount; t++) {
		// prepare samplers
		std::vector<std::vector<PixelSampler>> pixelSamplers;

		for (int i=0; i < tileWidth; i++) {
			pixelSamplers.push_back(std::vector<PixelSampler>());
			for (int j=0; j < cam.height; j++) {
				PixelSampler pixelSampler(samplesPerPixel);
				pixelSamplers[i].push_back(pixelSampler);
			}
		}

		resultTiles.push_back(std::async(renderTile, t, tiles[t], cam, scene, samplesPerPixel, pixelSamplers));
	}

	int k = 0;
	for (auto &result : resultTiles) {
		std::vector<std::vector<Vector3>> tile = result.get();
		
		
		for (int i = 0; i < tile.size(); i++ ) {
			for (int j =0; j < tile[0].size(); j++) {
				image[k*tile.size()+i][j] = tile[i][j];
			}
		}
		k++;
	}

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;

	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	image = ToneMapper::reinhard(image);

	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n ",cam.width,cam.height,255);
	for (int j=0;j<cam.height;j++) {
		for (int i=0;i<cam.width;i++) {
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


std::vector<std::vector<Vector3>> renderTile(int tileNumber, std::vector<std::vector<Vector3>> tile, Camera cam, std::vector<Shape*> scene, int samplesPerPixel, std::vector<std::vector<PixelSampler>> pixelSamplers) {

	Integrator pathTracer;

	int tileWidth = tile.size();
 
	for (int n=0; n < samplesPerPixel; n++) {
		for (int i=0; i < tile.size(); i++) {
			
			for (int j=0; j < tile[0].size(); j++) {

				Ray ray = cam.pixelToRay(pixelSamplers[i][j].getPixelSample(Vector2(tileNumber*tileWidth+i+0.5, j+0.5)));
				Vector3 colour = pathTracer.getRadiance(ray, 0, scene);
				tile[i][j] += colour/samplesPerPixel;
			}
		}
	}
	
	return tile;
}
