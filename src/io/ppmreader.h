#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "../core/smath.h"
#include "../core/image.h"

class PPMReader {
public:
    std::ifstream file;
    PPMReader(std::string filepath) : file(filepath) {};

    Image load() {
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
        std::vector<std::vector<Vector3>> image;
        std::vector<Vector3> row;

        while (std::getline(file, line))
        {
            std::istringstream iss3(line);

            int r;
            int g;
            int b;

            while (iss3 >> r >> g >> b) {
                row.push_back(Vector3(r, g, b)/double(max));

                if (row.size() >= width) {
                    image.push_back(row);
                    row.clear();
                }
            }            
        }

        return Image(image);
    };

};