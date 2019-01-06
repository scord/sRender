#include "core/smath.h"
#include "core/camera.h"
#include <stdio.h>
#include "core/sampler.h"
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
#include "io/plyreader.h"
#include "core/scene.h"
#include "samplers/pixelsampler.h"

#define PI 3.1415926536

std::vector<std::vector<Vector3>> renderTile(int tileNumber,  std::vector<std::vector<Vector3>> tile, Camera cam, Scene scene, int samplesPerPixel, int maxDepth);

int main() {

	Camera cam(Vector3(0,0,1.5), 180, 180, PI/4);

	Scene scene;

	PLYReader reader("/home/sam/dev/sRender/src/dragon.ply");

	//scene.add(reader.toObject(Vector3(-0.25,-1,-0.5), 4));

	KDTreeObject* bunny = reader.toKDTreeObject(Vector3(0,-1.5,0), 8);

	scene.add(bunny);

	// ROOM
	std::vector<Shape*> roomGeometry;


	roomGeometry.push_back(new Quad(Vector3(0,1,0), Vector3(0,-1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));
	roomGeometry.push_back(new Quad(Vector3(0,-1,0), Vector3(0,1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));
	roomGeometry.push_back(new Quad(Vector3(0,0,-1), Vector3(0,0,1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,1,0)));
	roomGeometry.push_back(new Quad(Vector3(1,0,0), Vector3(-1,0,0), Vector3(1,0,0), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));
	roomGeometry.push_back(new Quad(Vector3(-1,0,0), Vector3(1,0,0), Vector3(0,1,0), new DiffuseBrdf(), Vector2(1.0,1.0), Vector3(0,0,-1)));

	scene.add(new Object(roomGeometry, Vector3(0,0,0), 1));
	// BOX
	//scene.push_back(new Quad(Vector3(-0.5,0.1,-0.5), Vector3(0,1,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.25), Vector3(0,0,-1)));
	//scene.push_back(new Quad(Vector3(-0.75,-0.45,-0.5), Vector3(-1,0,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	//scene.push_back(new Quad(Vector3(-0.25,-0.45,-0.5), Vector3(1,0,0), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	//scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.25), Vector3(0,0,1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));
	//scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.75), Vector3(0,0,-1), Vector3(1,1,1), new DiffuseBrdf(), Vector2(0.25,0.55), Vector3(0,1,0)));

	// SPHERE

	std::vector<Shape*> sphereGeometry;

	//sphereGeometry.push_back(new Sphere(Vector3(0, 0, 0), Vector3(1,1,1), new DiffuseBrdf(), 1));
	//sphereGeometry.push_back(new Sphere(Vector3(0, 0.7, 0), Vector3(1,1,1), new DiffuseBrdf(), 0.7));
	//scene.add(new Object(sphereGeometry, Vector3(0,-0.5,0), 0.5));

	
	Triangle* triangleTest = new Triangle(Vector3(-1,-1,-1), Vector3(-1,1,-1), Vector3(1,1,-1), Vector3(10000,10000,10000), new DiffuseBrdf());
	triangleTest->isLight = true;
	//scene.push_back(triangleTest);
	// LIGHT

	Shape* areaLight = new Disc(Vector3(0,0.999,0), Vector3(0,-1,0), Vector3(10000,9800,9500), new DiffuseBrdf(), Vector2(0.5,0.5));
	areaLight->isLight = true;

	std::vector<Shape*> lightGeometry;
	lightGeometry.push_back(areaLight);
	
	scene.add(new Object(lightGeometry, Vector3(0,0,0), 1));

	std::vector<std::vector<Vector3>> image;

	for (int i=0; i < cam.width; i++) {
        image.push_back(std::vector<Vector3>());
        for (int j = 0; j < cam.height; j++) {
            image[i].push_back(Vector3(0,0,0));
        }
    }

	int samplesPerPixel = 512;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	// prepare tiles

	int tileCount = 6;
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
		std::vector<std::vector<Sampler>> pixelSamplers;

		for (int i=0; i < tileWidth; i++) {
			pixelSamplers.push_back(std::vector<Sampler>());
			for (int j=0; j < cam.height; j++) {
				Sampler pixelSampler(samplesPerPixel);
				pixelSamplers[i].push_back(pixelSampler);
			}
		}

		resultTiles.push_back(std::async(renderTile, t, tiles[t], cam, scene, samplesPerPixel, 5));
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
	//Sampler sampler(1);
	//sampler.generateTestImage(200, 200);

	return 0;
}


std::vector<std::vector<Vector3>> renderTile(int tileNumber, std::vector<std::vector<Vector3>> tile, Camera cam, Scene scene, int samplesPerPixel, int maxDepth) {

	

	int tileWidth = tile.size();
	int tileHeight = tile[0].size();

	PixelSampler pixelSampler(tileWidth, tileHeight, samplesPerPixel, maxDepth);

	Integrator pathTracer(pixelSampler, maxDepth);
	for (int i=0; i < tile.size(); i++) {
			
		for (int j=0; j < tile[0].size(); j++) {


			for (int n=0; n < samplesPerPixel; n++) {

				
				Ray ray = cam.pixelToRay(Vector2(tileNumber*tileWidth+i, j) + pixelSampler.getStratifiedSample().value);
				
				Vector3 colour = pathTracer.getRadiance(ray, 0, scene);
				tile[i][j] += colour/samplesPerPixel;

			}
		}
	}
	
	return tile;
}
