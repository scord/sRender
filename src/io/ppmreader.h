#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "../core/smath.h"
#include "../core/texture.h"

class PPMReader {
public:
    std::ifstream file;
    PPMReader(std::string filepath) : file(filepath) {};

    Texture load() {
        std::string line;
        std::getline(file, line);

        int nchannels = line.at(1);
        std::getline(file, line);
        std::istringstream iss(line);
        int width;
        int height;
        iss >> width >> height;
        std::getline(file, line);
        std::istringstream iss2(line);
        int max;
        iss2 >> max;
        std::vector<std::vector<vec3>> image;
        std::vector<vec3> row;

        while (std::getline(file, line))
        {
            std::istringstream iss3(line);

            int r;
            int g;
            int b;

            while (iss3 >> r >> g >> b) {
                row.push_back(vec3(r, g, b)/double(max));

                if (row.size() >= width) {
                    image.push_back(row);
                    row.clear();
                }
            }            
        }

        return Texture(image);
    };

};