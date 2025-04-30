#include "circle.h"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

Circle::Circle(double r, int n) {
    this->radius = r;
    this->num_points = n;
}

void Circle::saveToFile() {
    ofstream file("circle.dat");

    if (!file.is_open()) {
        cerr << "Failed to create file: circle.dat" << endl;
        return;
    }

    // Generate points for the circle
    for (int i = 0; i <= num_points; i++) {
        double angle = 2 * 3.14 * i / num_points; // Angle in radians
        double x = radius * cos(angle);
        double y = radius * sin(angle);
        file << x << " " << y << "\n";
    }

    file.close();
}

