#ifndef STL_PROCESSOR_H
#define STL_PROCESSOR_H

#include <vector>
#include <string>

std::vector<std::vector<std::vector<double>>> extractTriangles(const std::string &stlFile);
void writeDatFile(const std::string &datFile, const std::vector<std::vector<std::vector<double>>> &triangles);

#endif