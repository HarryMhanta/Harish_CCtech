#include "obj_processor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <set>
#include <tuple>
#include <algorithm>
#include <chrono>

// Helper function to compute normal
Vertex computeNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    Vertex u = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
    Vertex v = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };

    Vertex normal = {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    };

    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length != 0) {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }

    return normal;
}

void convertOBJtoSTL(const std::string& objFile, const std::string& stlFile) {
    std::ifstream inputFile(objFile);
    std::ofstream outputFile(stlFile);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening file!\n";
        return;
    }

    std::string line;
    std::vector<Vertex> vertices;
    std::set<std::tuple<int, int, int>> uniqueFaces;

    // Parse .obj file
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "f") {
            std::vector<int> faceIndices;
            std::string vert;
            while (ss >> vert) {
                int index = std::stoi(vert.substr(0, vert.find('/')));
                faceIndices.push_back(index - 1); // OBJ is 1-based
            }
            if (faceIndices.size() >= 3) {
                for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                    int a = faceIndices[0];
                    int b = faceIndices[i];
                    int c = faceIndices[i + 1];
                    // Ensure consistent ordering (helps eliminate duplicates)
                    std::vector<std::tuple<int, int, int>> perms = {
                        {a, b, c}, {a, c, b}, {b, a, c}, {b, c, a}, {c, a, b}, {c, b, a}
                    };
                    auto tri = *std::min_element(perms.begin(), perms.end());
                    
                    uniqueFaces.insert(tri);
                }
            }
        }
    }

    // Write ASCII STL
    outputFile << "solid converted_from_obj\n";
    for (const auto& face : uniqueFaces) {
        auto [i1, i2, i3] = face;
        Vertex v1 = vertices[i1];
        Vertex v2 = vertices[i2];
        Vertex v3 = vertices[i3];
        Vertex normal = computeNormal(v1, v2, v3);

        outputFile << "  facet normal " << normal.x << " " << normal.y << " " << normal.z << "\n";
        outputFile << "    outer loop\n";
        outputFile << "      vertex " << v1.x << " " << v1.y << " " << v1.z << "\n";
        outputFile << "      vertex " << v2.x << " " << v2.y << " " << v2.z << "\n";
        outputFile << "      vertex " << v3.x << " " << v3.y << " " << v3.z << "\n";
        outputFile << "    endloop\n";
        outputFile << "  endfacet\n";
    }
    outputFile << "endsolid converted_from_obj\n";

    inputFile.close();
    outputFile.close();
    std::cout << "Optimized STL conversion completed: " << stlFile << "\n";
}
