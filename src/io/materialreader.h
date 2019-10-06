#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "yaml-cpp/yaml.h"
#include "../core/material.h"
#include "../materials/fresnelblend.h"

class MaterialReader {
public:
    std::string filepath;
    MaterialReader(std::string filepath) : filepath(filepath) {};

    Material* load() {

        YAML::Node materialConfig = YAML::LoadFile(filepath);

        Material* material;

        if (materialConfig["type"].as<std::string>() == "FresnelBlend") {
            std::vector<double> diffuse = materialConfig["diffuse"].as<std::vector<double>>();
            std::vector<double> specular = materialConfig["specular"].as<std::vector<double>>();
            double roughness = materialConfig["roughness"].as<double>();
            material = new FresnelBlend(
                vec3(diffuse[0], diffuse[1], diffuse[2]),
                vec3(specular[0], specular[1], specular[2]),
                roughness
            );
        } else if (materialConfig["type"].as<std::string>() == "Specular") {
            material = new SpecularMaterial();
        }

        return material;
    };

};