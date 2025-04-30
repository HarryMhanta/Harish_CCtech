#include "stl_processor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<std::vector<std::vector<double>>> extractTriangles(const std::string &stlFile) {
    std::ifstream file(stlFile);
    if (!file) {
        throw std::runtime_error("Error opening STL file: " + stlFile);
    }

    std::vector<std::vector<std::vector<double>>> triangles;
    std::string line;
    std::vector<std::vector<double>> triangle;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        ss >> word;

        if (word == "vertex") {
            double x, y, z;
            ss >> x >> y >> z;
            triangle.push_back({x, y, z});

            if (triangle.size() == 3) {
                triangles.push_back(triangle);
                triangle.clear();
            }
        }
    }

    file.close();
    return triangles;
}

void writeDatFile(const std::string &datFile, const std::vector<std::vector<std::vector<double>>> &triangles) {
    std::ofstream file(datFile);
    if (!file) {
        throw std::runtime_error("Error opening DAT file: " + datFile);
    }

    for (const auto &triangle : triangles) {
        for (const auto &vertex : triangle) {
            file << vertex[0] << " " << vertex[1] << " " << vertex[2] << "\n";
        }
        file << triangle[0][0] << " " << triangle[0][1] << " " << triangle[0][2] << "\n";
        file << "\n\n";
    }

    file.close();
}
