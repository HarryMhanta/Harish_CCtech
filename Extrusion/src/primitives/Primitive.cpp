#include "Vertex.h"
#include "Edge.h"
#include "Face.h"

// Vertex class implementation
Vertex::Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

float Vertex::getX() const {
    return x;
}

float Vertex::getY() const {
    return y;
}

float Vertex::getZ() const {
    return z;
}

// Edge class implementation
Edge::Edge(const Vertex& start, const Vertex& end) : start(start), end(end) {}

float Edge::length() const {
    return sqrt(pow(end.getX() - start.getX(), 2) +
                pow(end.getY() - start.getY(), 2) +
                pow(end.getZ() - start.getZ(), 2));
}

// Face class implementation
Face::Face(const std::vector<Edge>& edges) : edges(edges) {}

QVector3D Face::normal() const {
    if (edges.size() < 3) return QVector3D(0, 0, 0); // Not enough edges to define a face
    QVector3D v1 = QVector3D(edges[1].getStart().getX() - edges[0].getStart().getX(),
                              edges[1].getStart().getY() - edges[0].getStart().getY(),
                              edges[1].getStart().getZ() - edges[0].getStart().getZ());
    QVector3D v2 = QVector3D(edges[2].getStart().getX() - edges[0].getStart().getX(),
                              edges[2].getStart().getY() - edges[0].getStart().getY(),
                              edges[2].getStart().getZ() - edges[0].getStart().getZ());
    return QVector3D::crossProduct(v1, v2).normalized();
}

// float Face::area() const {
//     // Implement area calculation based on edges
//     return 0.0f; // Placeholder
// }