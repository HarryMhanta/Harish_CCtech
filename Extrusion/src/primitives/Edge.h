#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"

class Edge {
public:
    Edge(const Vertex& start, const Vertex& end);
    
    float length() const;
    const Vertex& getStart() const;
    const Vertex& getEnd() const;

private:
    Vertex startVertex;
    Vertex endVertex;
};

#endif // EDGE_H