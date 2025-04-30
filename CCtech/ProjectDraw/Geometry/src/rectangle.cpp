// #include <iostream>
// #include <fstream>
// #include <cmath>
// #include "rectangle.h"
// using namespace std;

// Rectangle::Rectangle(double l, double b) : l(l), b(b) {}

// void Rectangle::generate() {
//     cin >> l >> b;

//     return Rectangle(l, b);
// }

// std::vector<std::pair<QVector3D, QVector3D>> Rectangle::getEdges() const {
//     //Define the 4 vertices of the rectangle
//     QVector3D v0(-l/2, -b/2, 0); //left bottom
//     QVector3D v1(l/2, -b/2, 0); //right bottom
//     QVector3D v2(l/2, b/2, 0); //right top
//     QVector3D v3(-l/2, b/2, 0); //left top

//     //Define the edges as pairs of vertices
//     return std::vector<std::pair<QVector3D, QVector3D>>{
//         {v0, v1}, {v1, v2}, {v2, v3}, {v3, v0} //rectangle edges
//     };
// }
