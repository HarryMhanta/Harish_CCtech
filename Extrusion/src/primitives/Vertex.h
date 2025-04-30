#ifndef VERTEX_H
#define VERTEX_H

class Vertex {
private:
    float x;
    float y;
    float z;

public:
    Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    
    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);
};

#endif // VERTEX_H