#include "core/smath.h"
#include "core/camera.h"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "core/geometry.h"
#include "core/integrator.h"
#include <thread>
#include <future>
#include <chrono>
#include <iostream>
#include "core/tonemapper.h"
#include "io/plyreader.h"
#include "core/scene.h"
#include "samplers/sampler.h"
#include "materials/diffuse.h"
#include "materials/specular.h"
#include "materials/mirror.h"
#include "materials/orennayar.h"
#include "materials/fresnelblend.h"
#include "materials/ggxmaterial.h"
#include "materials/emissive.h"

#include "io/ppmreader.h"
#include "core/texture.h"
#include "io/scenereader.h"
#define PI 3.1415926536

volatile std::atomic<int> progress;
int tileCount = 64;

std::vector<std::vector<vec3>> renderTile(int tileNumber,  std::vector<std::vector<vec3>> tile, Camera cam, Scene scene, int samplesPerPixel, int maxDepth);

int main() {

	SceneReader sceneReader("/home/sam/dev/sRender/scenes/box.yaml");
	//sceneReader.load();

	Camera cam(vec3(0,0,3), 1280, 1280, PI/6);

	Scene scene;

	progress = 0;

	scene = sceneReader.load();
	std::vector<std::vector<vec3>> texture;

	for (int i=0; i < cam.width; i++) {
        texture.push_back(std::vector<vec3>());
        for (int j = 0; j < cam.height; j++) {
            texture[i].push_back(vec3(0,0,0));
        }
    }

	int samplesPerPixel = 16;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	// prepare tiles

	
	int tileWidth = cam.width/tileCount;
	std::vector<std::vector<std::vector<vec3>>> tiles;

	for (int t = 0; t < tileCount; t++) {

		std::vector<std::vector<vec3>> tile;
		
		for (int i = 0; i < tileWidth; i++) {
			tile.push_back(std::vector<vec3>());
			for (int j =0; j < cam.height; j++) {
				tile[i].push_back(vec3(0,0,0));
			}
		}
		tiles.push_back(tile);
	}


	std::vector<std::future<std::vector<std::vector<vec3>>>> resultTiles;

	for (int t = 0; t < tileCount; t++) {
		resultTiles.push_back(std::async(renderTile, t, tiles[t], cam, scene, samplesPerPixel, 8));
	}

	int k = 0;
	for (auto &result : resultTiles) {
		std::vector<std::vector<vec3>> tile = result.get();
		
		
		for (int i = 0; i < tile.size(); i++ ) {
			for (int j =0; j < tile[0].size(); j++) {
				texture[k*tile.size()+i][j] = tile[i][j];
			}
		}
		k++;
	}


	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;

	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	texture = ToneMapper::reinhard(texture);

	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n ",cam.width,cam.height,65535);
	for (int j=0;j<cam.height;j++) {
		for (int i=0;i<cam.width;i++) {

			fprintf(f,"%d %d %d ", (int)(65535*texture[i][j].x),
								   (int)(65535*texture[i][j].y),
								   (int)(65535*texture[i][j].z));
		}
		fprintf(f, "\n");
	}
	fclose(f);

	//sampler sampler2(4096);
	//Sampler sampler(1);
	//sampler.generateTestImage(200, 200);

	return 0;
}


std::vector<std::vector<vec3>> renderTile(int tileNumber, std::vector<std::vector<vec3>> tile, Camera cam, Scene scene, int samplesPerPixel, int maxDepth) {

	

	int tileWidth = tile.size();
	int tileHeight = tile[0].size();

	Sampler sampler = Sampler(tileWidth*tileHeight, samplesPerPixel, maxDepth);
	double a = 500.0;
	Integrator pathTracer(maxDepth, scene, sampler);
	for (int i=0; i < tile.size(); i++) {
			
		for (int j=0; j < tile[0].size(); j++) {
			for (int n=0; n < samplesPerPixel; n++) {
				Ray ray = cam.pixelToRay(vec2(tileNumber*tileWidth+i, j) + (sampler.getStratifiedSample().value));	
				//Ray ray = cam.pixelToRay(vec2(tileNumber*tileWidth+i, j) + (sampler->getStratifiedSample().value)*a, vec2(tileNumber*tileWidth+i, j) + vec2(0.5, 0.5), 1.0, 2.5);	
				vec3 colour = pathTracer.getRadiance(ray, maxDepth);
				tile[i][j] += colour/samplesPerPixel;
			}
		}
	}
	
	progress += 1;

	std::cout << 100*progress/tileCount <<  "%\n";
	return tile;
}
