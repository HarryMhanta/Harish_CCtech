#include "cylinder.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

template <typename T>
Cylinder<T>::Cylinder(T radius, T height)
    : radius(radius), height(height) {}

template <typename T>
void Cylinder<T>::saveToFile() {
    ofstream file("cylinder.dat");
    auto edges = getEdges();

    for (const auto& [v1, v2] : edges) {
        file << v1.x() << " " << v1.y() << " " << v1.z() << "\n";
        file << v2.x() << " " << v2.y() << " " << v2.z() << "\n\n";
    }

    file.close();
}

template <typename T>
std::shared_ptr<Parent<T>> Cylinder<T>::generate() {
    cout << "Enter the radius and height of the cylinder: ";
    cin >> radius >> height;

    auto cylinder = std::make_shared<Cylinder<T>>(radius, height);
    cylinder->saveToFile();

    return cylinder;
}

template <typename T>
std::vector<float> Cylinder<T>::getVertexBuffer() const {
    const int segments = 100;
    const T pi = 3.14159265358979323846;
    std::vector<float> vertices;  // Change from QVector3D to float array

    for (int i = 0; i < segments; ++i) {
        T angle = 2 * pi * i / segments;
        vertices.push_back(radius * std::cos(angle));   // x coordinate
        vertices.push_back(radius * std::sin(angle));   // y coordinate
        vertices.push_back(-height / 2);                 // z coordinate (bottom)

        vertices.push_back(radius * std::cos(angle));   // x coordinate
        vertices.push_back(radius * std::sin(angle));   // y coordinate
        vertices.push_back(height / 2);                 // z coordinate (top)
    }

    return vertices;
}


template <typename T>
std::vector<unsigned int> Cylinder<T>::getEdgeIndices() const {
    const int segments = 100;
    std::vector<unsigned int> indices;

    for (int i = 0; i < segments; ++i) {
        int base = i * 2;
        int next = ((i + 1) % segments) * 2;

        indices.push_back(base);       // vertical
        indices.push_back(base + 1);

        indices.push_back(base);       // bottom ring
        indices.push_back(next);

        indices.push_back(base + 1);   // top ring
        indices.push_back(next + 1);
    }

    return indices;
}


template <typename T>
std::vector<std::pair<QVector3D, QVector3D>> Cylinder<T>::getEdges() const {
    std::vector<std::pair<QVector3D, QVector3D>> edges;
    auto vertices = getVertexBuffer();
    const int segments = 100;

    for (int i = 0; i < segments; ++i) {
        int bottomIndex = i * 2;
        int topIndex = bottomIndex + 1;

        // Get current and next segment indices (for wrap-around)
        int nextBottomIndex = ((i + 1) % segments) * 2;
        int nextTopIndex = nextBottomIndex + 1;

        QVector3D bottom(vertices[bottomIndex * 3], vertices[bottomIndex * 3 + 1], vertices[bottomIndex * 3 + 2]);
        QVector3D top(vertices[topIndex * 3], vertices[topIndex * 3 + 1], vertices[topIndex * 3 + 2]);
        QVector3D nextBottom(vertices[nextBottomIndex * 3], vertices[nextBottomIndex * 3 + 1], vertices[nextBottomIndex * 3 + 2]);
        QVector3D nextTop(vertices[nextTopIndex * 3], vertices[nextTopIndex * 3 + 1], vertices[nextTopIndex * 3 + 2]);

        // Add vertical edge
        edges.emplace_back(bottom, top);

        // Add edge on bottom circle
        edges.emplace_back(bottom, nextBottom);

        // Add edge on top circle
        edges.emplace_back(top, nextTop);
    }

    return edges;
}



// Explicit instantiation
template class Cylinder<float>;
template class Cylinder<double>;
