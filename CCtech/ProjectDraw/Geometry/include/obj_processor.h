#ifndef OBJ_PROCESSOR_H
#define OBJ_PROCESSOR_H

#include <vector>
#include <string>

struct Vertex {
    double x, y, z;
};

void convertOBJtoSTL(const std::string& objFile, const std::string& stlFile);
Vertex computeNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3);

#endif // OBJ_PROCESSOR_H