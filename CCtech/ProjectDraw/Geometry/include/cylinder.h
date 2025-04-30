#ifndef CYLINDER_H
#define CYLINDER_H

#pragma once
#include "parent.h"
#include <vector>
#include <utility>
#include <QVector3D>

template <typename T>
class Cylinder : public Parent<T> {
    T radius, height;

public:
    Cylinder(T radius, T height);
    std::shared_ptr<Parent<T>> generate() override;
    std::vector<float> getVertexBuffer() const ;
    std::vector<unsigned int> getEdgeIndices() const ;
    std::vector<std::pair<QVector3D, QVector3D>> getEdges() const ;
    void saveToFile() override;
};

#endif // CYLINDER_H