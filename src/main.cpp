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

#define PI 3.1415926536

std::vector<std::vector<Vector3>> renderTile(int tileNumber,  std::vector<std::vector<Vector3>> tile, Camera cam, Scene scene, int samplesPerPixel, int maxDepth);

int main() {

	Camera cam(Vector3(0,0,3), 640, 640, PI/6);

	Scene scene;

	//PLYReader reader("/home/sam/dev/sRender/src/dragon.ply");

	//scene.add(reader.toObject(Vector3(-0.25,-1,-0.5), 4));

	//KDTreeObject* bunny = reader.toKDTreeObject(Vector3(0,-1.45,0), 8);

	//scene.add(bunny);

	// ROOM
	std::vector<Shape*> roomGeometry;


	roomGeometry.push_back(new Quad(Vector3(0,1,0), Vector3(0,-1,0), Vector2(1.0,1.0), Vector3(0,0,-1)));
	roomGeometry.push_back(new Quad(Vector3(0,-1,0), Vector3(0,1,0), Vector2(1.0,1.0), Vector3(0,0,-1)));
	roomGeometry.push_back(new Quad(Vector3(0,0,-1), Vector3(0,0,1), Vector2(1.0,1.0), Vector3(0,1,0)));
	roomGeometry.push_back(new Quad(Vector3(0,0,1), Vector3(0,0,-1), Vector2(1.0,1.0), Vector3(0,1,0)));
	
	std::vector<Shape*> redRoomGeometry;
	redRoomGeometry.push_back(new Quad(Vector3(1,0,0), Vector3(-1,0,0), Vector2(1.0,1), Vector3(0,0,-1)));

	std::vector<Shape*> greenRoomGeometry;
	greenRoomGeometry.push_back(new Quad(Vector3(-1,0,0), Vector3(1,0,0), Vector2(1.0,1), Vector3(0,0,-1)));
	
	OrenNayarMaterial* on = new OrenNayarMaterial(Vector3(0.9, 0.1, 0.1), 0.2);


	scene.add(new Object(redRoomGeometry, Vector3(0,0,0), 1, new OrenNayarMaterial(Vector3(0.5,0.1,0.1), 0.15)));
	scene.add(new Object(greenRoomGeometry, Vector3(0,0,0), 1, new OrenNayarMaterial(Vector3(0.1,0.5,0.1), 0.15)));
	scene.add(new Object(roomGeometry, Vector3(0,0,0), 1, new OrenNayarMaterial(Vector3(0.4,0.4,0.4), 0.15)));
	// BOX
	//scene.push_back(new Quad(Vector3(-0.5,0.1,-0.5), Vector3(0,1,0), Vector3(1,1,1), new DiffuseMaterial(), Vector2(0.25,0.25), Vector3(0,0,-1)));
	//scene.push_back(new Quad(Vector3(-0.75,-0.45,-0.5), Vector3(-1,0,0), Vector3(1,1,1), new DiffuseMaterial(), Vector2(0.25,0.55), Vector3(0,1,0)));
	//scene.push_back(new Quad(Vector3(-0.25,-0.45,-0.5), Vector3(1,0,0), Vector3(1,1,1), new DiffuseMaterial(), Vector2(0.25,0.55), Vector3(0,1,0)));
	//scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.25), Vector3(0,0,1), Vector3(1,1,1), new DiffuseMaterial(), Vector2(0.25,0.55), Vector3(0,1,0)));
	//scene.push_back(new Quad(Vector3(-0.5,-0.45,-0.75), Vector3(0,0,-1), Vector3(1,1,1), new DiffuseMaterial(), Vector2(0.25,0.55), Vector3(0,1,0)));

	// SPHERE


//	scene.add(new Object(std::vector<Shape*>{new Sphere(Vector3(0.4, -0.6, 0.5), 0.4)}, Vector3(0,0,0), 1, new SpecularMaterial(Vector3(1,1,1))));
//scene.add(new Object(std::vector<Shape*>{new Sphere(Vector3(0.5, 0.3, 0.2), 0.3)}, Vector3(0,0,0), 1, new SpecularMaterial(Vector3(1,1,1))));
//	scene.add(new Object(std::vector<Shape*>{new Sphere(Vector3(0.2, -0.1, -0.5), 0.3)}, Vector3(0,0,0), 1, new SpecularMaterial(Vector3(1,1,1))));
	
	scene.add(new Object(std::vector<Shape*>{new Sphere(Vector3(-0.3, -0.6, -0.35), 0.4)}, Vector3(0,0,0), 1, new FresnelBlend(Vector3(0.8,0.3,0.1), Vector3(0.4,0.4,0.4), 0.6)));
	scene.add(new Object(std::vector<Shape*>{new Sphere(Vector3(0.4, -0.6, 0.2), 0.4)}, Vector3(0,0,0), 1, new FresnelBlend(Vector3(0.2,0.1,0.5), Vector3(0.4,0.4,0.4), 0.4)));
	//scene.add(new Object(std::vector<Shape*>{new Sphere(Vector3(0.4, -0.6, 0.2), 0.4)}, Vector3(0,0,0), 1, new GGXMaterial(Vector3(0.2,0.1,0.5), Vector3(0.4,0.4,0.4), 0.1)));

	Shape* areaLight = new Disc(Vector3(-0.5,0.999,0.5), Vector3(0,-1,0), Vector2(0.25,0.25));
	Shape* areaLight2 = new Disc(Vector3(0.5,0.999,0.5), Vector3(0,-1,0), Vector2(0.25,0.25));
	Shape* areaLight3 = new Disc(Vector3(-0.5,0.999,-0.5), Vector3(0,-1,0), Vector2(0.25,0.25));
	Shape* areaLight4 = new Disc(Vector3(0.5,0.999,-0.5), Vector3(0,-1,0), Vector2(0.25,0.25));

	std::vector<Shape*> lightGeometry;
	lightGeometry.push_back(areaLight);
	std::vector<Shape*> lightGeometry2;
	lightGeometry2.push_back(areaLight2);
	std::vector<Shape*> lightGeometry3;
	lightGeometry3.push_back(areaLight3);
	std::vector<Shape*> lightGeometry4;
	lightGeometry4.push_back(areaLight4);
	
	scene.addLight(new Object(lightGeometry, Vector3(0,0,0), 1, new DiffuseMaterial(Vector3(1,1,1), Vector3(900,500,500))));
	scene.addLight(new Object(lightGeometry2, Vector3(0,0,0), 1, new DiffuseMaterial(Vector3(1,1,1), Vector3(1000,900,80))));
	scene.addLight(new Object(lightGeometry3, Vector3(0,0,0), 1, new DiffuseMaterial(Vector3(1,1,1), Vector3(100,900,800))));
	scene.addLight(new Object(lightGeometry4, Vector3(0,0,0), 1, new DiffuseMaterial(Vector3(1,1,1), Vector3(500,900,500))));

	std::vector<std::vector<Vector3>> image;

	for (int i=0; i < cam.width; i++) {
        image.push_back(std::vector<Vector3>());
        for (int j = 0; j < cam.height; j++) {
            image[i].push_back(Vector3(0,0,0));
        }
    }

	int samplesPerPixel = 256;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	// prepare tiles

	int tileCount = 16;
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
		resultTiles.push_back(std::async(renderTile, t, tiles[t], cam, scene, samplesPerPixel, 3));
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
	fprintf(f, "P3\n%d %d\n%d\n ",cam.width,cam.height,65535);
	for (int j=0;j<cam.height;j++) {
		for (int i=0;i<cam.width;i++) {

			fprintf(f,"%d %d %d ", (int)(65535*image[i][j].x),
								   (int)(65535*image[i][j].y),
								   (int)(65535*image[i][j].z));
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

	Sampler* pixelSampler = new Sampler(tileWidth*tileHeight, samplesPerPixel, maxDepth);
	double a = 500.0;
	Integrator pathTracer(maxDepth);
	for (int i=0; i < tile.size(); i++) {
			
		for (int j=0; j < tile[0].size(); j++) {
			for (int n=0; n < samplesPerPixel; n++) {
				Ray ray = cam.pixelToRay(Vector2(tileNumber*tileWidth+i, j) + (pixelSampler->getStratifiedSample().value));	
				//Ray ray = cam.pixelToRay(Vector2(tileNumber*tileWidth+i, j) + (pixelSampler->getStratifiedSample().value)*a, Vector2(tileNumber*tileWidth+i, j) + Vector2(0.5, 0.5), 1.0, 2.5);	
				Vector3 colour = pathTracer.getRadiance(ray, maxDepth, &scene, pixelSampler);
				tile[i][j] += colour/samplesPerPixel;
			}
		}
	}

	delete pixelSampler;
	
	return tile;
}
