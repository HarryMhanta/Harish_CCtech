#ifndef POINT_H
#define POINT_H

class point {
    private:
        double x, y, z;
    public:
        point(double x, double y, double z) : x(x), y(y), z(z) {}

        double getX() const { return x;}
        double getY() const { return y;}
        double getZ() const { return z;}

        void setX(double x) { this->x = x;}
        void setY(double y) { this->y = y;}
        void setZ(double z) { this->z = z;}
    

}





#endif // POINT_H