#include "Cube.h"
#include <vector>
#include <iostream>
#include <QVector3D>
using namespace std;

// Constructor
template <typename T>
Cube<T>::Cube(T l, T b, T h) : l(l), b(b), h(h) {}

// Generate a cube with user input dimensions
template <typename T>
std::shared_ptr<Parent<T>> Cube<T>::generate() {
    //cout << "Enter the length, breadth, and height of the cube: ";
    cin >> l >> b >> h;

    return std::make_shared<Cube<T>>(l, b, h);
}
template <typename T>
void Cube<T>::saveToFile() {
}

// Get edges as QVector3D pairs directly
template <typename T>
std::vector<std::pair<QVector3D, QVector3D>> Cube<T>::getEdges() const {
    // Define the 8 vertices of the cube
    QVector3D v0(-l / 2, -b / 2, -h / 2); // Left bottom front
    QVector3D v1( l / 2, -b / 2, -h / 2); // Right bottom front
    QVector3D v2( l / 2,  b / 2, -h / 2); // Right top front
    QVector3D v3(-l / 2,  b / 2, -h / 2); // Left top front
    QVector3D v4(-l / 2, -b / 2,  h / 2); // Left bottom back
    QVector3D v5( l / 2, -b / 2,  h / 2); // Right bottom back
    QVector3D v6( l / 2,  b / 2,  h / 2); // Right top back
    QVector3D v7(-l / 2,  b / 2,  h / 2); // Left top back

    // Define edges as pairs of vertices
    return {
        {v0, v1}, {v1, v2}, {v2, v3}, {v3, v0}, // Bottom face
        {v4, v5}, {v5, v6}, {v6, v7}, {v7, v4}, // Top face
        {v0, v4}, {v1, v5}, {v2, v6}, {v3, v7}  // Vertical edges
    };
}

// Explicit template instantiation
template class Cube<float>;
template class Cube<double>;
