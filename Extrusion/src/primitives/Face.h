#ifndef FACE_H
#define FACE_H

#include <QVector>
#include "Edge.h"

class Face {
public:
    Face(const QVector<Edge>& edges);
    
    QVector<Edge> getEdges() const;
    //QVector3D calculateNormal() const;
    //float calculateArea() const;

private:
    QVector<Edge> edges;
};

#endif // FACE_H