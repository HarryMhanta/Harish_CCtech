#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include <vector>
#include <QVector3D>
#include "Parent.h"

template <typename T>
class Sphere : public Parent<T> {
public:
    Sphere(T r);  // Only radius now
    void saveToFile() override;
    std::shared_ptr<Parent<T>> generate() override;
    std::vector<std::pair<QVector3D, QVector3D>> getEdges() const;

private:
    T r;
};

#endif
// SPHERE_H