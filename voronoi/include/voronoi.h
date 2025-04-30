#pragma once

#include <vector>

struct Point {
    double x, y;
};

struct Edge {
    Point a, b;
};

using Polygon = std::vector<Point>;

std::vector<Polygon> computeVoronoiCells(std::vector<Point> sites, double pad);
