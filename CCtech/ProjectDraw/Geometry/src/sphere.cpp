#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <memory>
#include "sphere.h"
#include <QVector3D>

using namespace std;

template <typename T>
Sphere<T>::Sphere(T r) : r(r) {}

template <typename T>
void Sphere<T>::saveToFile() {
    ofstream file("sphere.dat");
    const double pi = 3.14159265359;
    double theta, phi;

    for (int i = 0; i < 100; i++) {
        theta = 2 * pi * i / 100;
        for (int j = 0; j < 100; j++) {
            phi = pi * j / 100;
            file << r * sin(phi) * cos(theta) << " "
                 << r * sin(phi) * sin(theta) << " "
                 << r * cos(phi) << "\n";
        }
        file << "\n";
    }

    file.close();
}

template <typename T>
std::shared_ptr<Parent<T>> Sphere<T>::generate() {
    T r;
    cout << "Enter radius of sphere: ";
    cin >> r;

    auto sphere = std::make_shared<Sphere<T>>(r);
    sphere->saveToFile();

    std::ifstream sphereFile("sphere.dat");
    std::ofstream roboFile("robot.dat", std::ios::app);
    if (sphereFile.is_open() && roboFile.is_open()) {
        roboFile << sphereFile.rdbuf();
        roboFile << "\n\n";
    } else {
        std::cerr << "Error opening file." << std::endl;
    }

    sphereFile.close();
    roboFile.close();

    return sphere;
}

template <typename T>
std::vector<std::pair<QVector3D, QVector3D>> Sphere<T>::getEdges() const {
    std::vector<std::pair<QVector3D, QVector3D>> edges;
    const int slices = 20;
    const int stacks = 20;
    const float pi = 3.14159265359f;

    for (int i = 0; i < slices; ++i) {
        float theta1 = (float)i * 2.0f * pi / slices;
        float theta2 = (float)(i + 1) * 2.0f * pi / slices;

        for (int j = 0; j < stacks; ++j) {
            float phi1 = (float)j * pi / stacks;
            float phi2 = (float)(j + 1) * pi / stacks;

            QVector3D p1(r * sin(phi1) * cos(theta1),
                         r * sin(phi1) * sin(theta1),
                         r * cos(phi1));

            QVector3D p2(r * sin(phi2) * cos(theta1),
                         r * sin(phi2) * sin(theta1),
                         r * cos(phi2));

            QVector3D p3(r * sin(phi1) * cos(theta2),
                         r * sin(phi1) * sin(theta2),
                         r * cos(phi1));

            edges.emplace_back(p1, p2);
            edges.emplace_back(p1, p3);
        }
    }

    return edges;
}

template class Sphere<double>;
template class Sphere<float>;
