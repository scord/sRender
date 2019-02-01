#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../core/geometry.h"
#include "../brdfs/specular.h"
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
    Object* toObject(Vector3 pos, double scale) {
        return new Object(toTriangles(), pos, scale);
    }
    KDTreeObject* toKDTreeObject(Vector3 pos, double scale) {
        std::vector<Shape*> test = toTriangles();

        return new KDTreeObject(test, pos, scale);
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

        std::vector<Vector3> vertices;

        for (int i = 0; i < vn; i++){
            std::getline(file, line);
            std::istringstream iss(line);

            double x, y, z;
            iss >> x >> y >> z;

            vertices.push_back(Vector3(x,y,z));
        }

       

        for (int i = 0; i < tn; i++) {
            std::getline(file, line);
            std::istringstream iss(line);

            int n, v0, v1, v2;
            iss >> n >> v0 >> v1 >> v2;

            Triangle* triangle = new Triangle(vertices[v0],
                                             vertices[v1],
                                             vertices[v2],
                                             Vector3(1, 1, 1),
                                             new SpecularBrdf());



            triangles.push_back(triangle);
        }

        return triangles;

    };
};