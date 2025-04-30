// #ifndef CUBE_H
// #define CUBE_H
// #include "parent.h"

// template <typename T>
// class Cube:public Parent<T> {
//     T l, b, h, cx, cy, cz;
// public:
//     void getVerticesAndEdges(std::vector<QVector3D>& outVertices, std::vector<std::pair<int, int>>& outEdges);
//     Cube(T l, T b, T h, T cx, T cy, T cz);
//     Cube() = default;
//     void saveToFile() override;
//     //void plot();
//     std::shared_ptr<Parent<T>> generate() override;
// };

// #endif
#ifndef CUBE_H
#define CUBE_H

#include "parent.h"
#include <vector>
#include <utility>  // for std::pair
#include <QVector3D>


template <typename T>
class Cube : public Parent<T> {
    T l, b, h;

public:
    Cube(T l, T b, T h);
    Cube() = default;
    std::vector<std::pair<QVector3D, QVector3D>> getEdges() const;
    //std::vector<float> getVertexBuffer() const;
    //std::vector<unsigned int> getEdgeIndices() const;

    void saveToFile() override;
    std::shared_ptr<Parent<T>> generate() override;
};

#endif
