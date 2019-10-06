#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../core/geometry.h"
#include "../materials/specular.h"
#include "../core/scene.h"
#include <memory>

class PLYReader {
public:
    std::ifstream file;
    PLYReader(std::string filepath) : file(filepath) {};

    int getN() {
        bool foundElement = false;
        int n;
        std::string line;
        while (!foundElement) {
            std::getline(file, line);
            std::istringstream iss(line);
            
            std::string key;
            std::string data;
            iss >> key;

            if (key == "element") {
                std::string type;    
                iss >> type >> n;
                foundElement = true;
            }
        }
        return n;
    }
    Object* toObject(vec3 pos, double scale) {
        return new Object(toTriangles(), pos, scale, new SpecularMaterial());
    }
     Object* toObject(vec3 pos, double scale, Material* material) {
        return new Object(toTriangles(), pos, scale, material);
    }
    KDTreeObject* toKDTreeObject(vec3 pos, double scale) {
        std::vector<Shape*> test = toTriangles();

        return new KDTreeObject(test, pos, scale, new SpecularMaterial());
    }
    KDTreeObject* toKDTreeObject(vec3 pos, double scale, Material* material) {
        std::vector<Shape*> test = toTriangles();

        return new KDTreeObject(test, pos, scale, material);
    }
    std::vector<Shape*> toTriangles() {
        std::vector<Shape*> triangles;

        std::string line;
        std::getline(file, line);
        if (line != "ply") {
            printf("Not a PLY file");
            return triangles;
        }
       
        int vn = getN();
        int tn = getN();

        bool foundData = false;
        while (line != "end_header") { std::getline(file, line); }

        // Elements

        std::vector<vec3> vertices;

        for (int i = 0; i < vn; i++){
            std::getline(file, line);
            std::istringstream iss(line);

            double x, y, z;
            iss >> x >> y >> z;

            vertices.push_back(vec3(x,y,z));
        }

       

        for (int i = 0; i < tn; i++) {
            std::getline(file, line);
            std::istringstream iss(line);

            int n, v0, v1, v2;
            iss >> n >> v0 >> v1 >> v2;

            Triangle* triangle = new Triangle(vertices[v0],
                                             vertices[v1],
                                             vertices[v2]
                                             );



            triangles.push_back(triangle);
        }

        return triangles;

    };
};