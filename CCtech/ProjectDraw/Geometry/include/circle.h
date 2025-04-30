#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
    double radius;
    int num_points; // Number of points to approximate the circle
public:
    Circle(double r, int n);
    void saveToFile();

};

#endif