#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "yaml-cpp/yaml.h"
#include "../core/scene.h"
#include "materialreader.h"

class SceneReader {
public:
    std::string filepath;
    SceneReader(std::string filepath) : filepath(filepath) {};

    Scene load() {

        YAML::Node sceneConfig = YAML::LoadFile(filepath);

        Scene scene;
        
        for (auto object : sceneConfig["objects"]) {
            if (object["type"].as<std::string>() == "File") {
                std::vector<double> pos = object["position"].as<std::vector<double>>();
                double scale = object["scale"].as<double>();
                std::string object_path = object["filepath"].as<std::string>();

                PLYReader reader(object_path);

                std::string material_path = object["material"].as<std::string>();
                Material* material = MaterialReader(material_path).load();
                

	            KDTreeObject* obj = reader.toKDTreeObject(Vector3(pos[0], pos[1], pos[2]), scale, material);

                
                scene.add( obj );
            }
            if (object["type"].as<std::string>() == "Sphere") {
                std::vector<double> pos = object["position"].as<std::vector<double>>();
                std::vector<double> col = object["colour"].as<std::vector<double>>();
                std::string material_path = object["material"].as<std::string>();
                Material* material = MaterialReader(material_path).load();
                
                
                scene.add(new Object(
                    std::vector<Shape*>{new Sphere(
                        Vector3(pos[0], pos[1], pos[2]),
                        object["radius"].as<double>()
                    )},
                    Vector3(0,0,0),
                    1,
                    material
                ));
            } else if (object["type"].as<std::string>() == "Quad") {
                std::vector<double> pos = object["position"].as<std::vector<double>>();
                std::vector<double> col = object["colour"].as<std::vector<double>>();
                std::vector<double> norm = object["normal"].as<std::vector<double>>();
                std::vector<double> right = object["right"].as<std::vector<double>>(); 
                std::vector<double> scale = object["scale"].as<std::vector<double>>();
                scene.add(new Object(
                    std::vector<Shape*>{new Quad(
                        Vector3(pos[0], pos[1], pos[2]),
                        Vector3(norm[0], norm[1], norm[2]),
                        Vector2(scale[0], scale[1]),
                        Vector3(right[0], right[1], right[2])
                    )},
                    Vector3(0,0,0),
                    1,
                    new OrenNayarMaterial(Vector3(col[0],col[1],col[2]),0.95)
                ));
            } else if (object["type"].as<std::string>() == "Triangle") {
                std::vector<double> v0 = object["v0"].as<std::vector<double>>();
                std::vector<double> v1 = object["v1"].as<std::vector<double>>();
                std::vector<double> v2 = object["v2"].as<std::vector<double>>();

                std::string material_path = object["material"].as<std::string>();
                Material* material = MaterialReader(material_path).load();

                scene.add(new Object(
                    std::vector<Shape*>{new Triangle(
                        Vector3(v0[0], v0[1], v0[2]),
                        Vector3(v1[0], v1[1], v1[2]),
                        Vector3(v2[0], v2[1], v2[2])
                    )},
                    Vector3(0,0,0),
                    1,
                    material
                ));
            }
        }

        for (auto object : sceneConfig["lights"]) {
            if (object["type"].as<std::string>() == "Disc") {
                std::vector<double> pos = object["position"].as<std::vector<double>>();
                std::vector<double> col = object["colour"].as<std::vector<double>>();
                std::vector<double> norm = object["normal"].as<std::vector<double>>();
                double intensity = object["intensity"].as<double>();
                double radius = object["radius"].as<double>();
                scene.addLight(new Object(
                    std::vector<Shape*>{new Disc(
                        Vector3(pos[0], pos[1], pos[2]),
                        Vector3(norm[0], norm[1], norm[2]),
                        Vector2(radius, radius)
                    )},
                    Vector3(0,0,0),
                    1,
                    new EmissiveMaterial(Vector3(col[0],col[1],col[2])*intensity)
                ));
            }
        }

        return scene;
    };

};