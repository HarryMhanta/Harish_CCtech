#include "voronoi.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>

using namespace std;

double EPS = 1e-9;

// Cross product of (O->A) x (O->B)
double cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Dot product
double dot(const Point& A, const Point& B) {
    return A.x * B.x + A.y * B.y;
}

// Vector subtraction
Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

// Vector addition
Point operator+(const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y};
}

// Scalar multiplication
Point operator*(const Point& a, double t) {
    return {a.x * t, a.y * t};
}

// Line intersection using parametric form
bool lineIntersect(const Point& A, const Point& B, const Point& C, const Point& D, Point& out) {
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1 * A.x + b1 * A.y;

    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2 * C.x + b2 * C.y;

    double det = a1 * b2 - a2 * b1;
    if (abs(det) < EPS) return false; // Parallel

    out.x = (b2 * c1 - b1 * c2) / det;
    out.y = (a1 * c2 - a2 * c1) / det;

    // Check if within segments
    if (min(A.x, B.x) - EPS <= out.x && out.x <= max(A.x, B.x) + EPS &&
        min(A.y, B.y) - EPS <= out.y && out.y <= max(A.y, B.y) + EPS &&
        min(C.x, D.x) - EPS <= out.x && out.x <= max(C.x, D.x) + EPS &&
        min(C.y, D.y) - EPS <= out.y && out.y <= max(C.y, D.y) + EPS) {
        return true;
    }
    return false;
}

// Perpendicular bisector between two points
Edge perpendicularBisector(const Point& p1, const Point& p2) {
    Point mid = {(p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0};
    Point dir = {p2.y - p1.y, p1.x - p2.x}; // Rotate 90 degrees

    Point p3 = mid + dir;
    Point p4 = mid - dir;

    return {p3, p4};
}

// Determine if a point is on the left side of a bisector
bool isLeft(const Edge& bisector, const Point& site, const Point& p) {
    return cross(bisector.a, bisector.b, p) * cross(bisector.a, bisector.b, site) > 0;
}

// Clip polygon with a half-plane defined by the bisector
Polygon clipPolygon(const Polygon& poly, const Edge& bisector, const Point& site) {
    Polygon newPoly;
    int n = poly.size();

    for (int i = 0; i < n; ++i) {
        Point A = poly[i];
        Point B = poly[(i + 1) % n];

        bool insideA = isLeft(bisector, site, A);
        bool insideB = isLeft(bisector, site, B);

        if (insideA) newPoly.push_back(A);

        if (insideA != insideB) {
            Point inter;
            if (lineIntersect(A, B, bisector.a, bisector.b, inter)) {
                newPoly.push_back(inter);
            }
        }
    }
    return newPoly;
}

// Compute Voronoi cells
vector<Polygon> computeVoronoiCells(vector<Point> sites, double pad) {
    double xl = numeric_limits<double>::max(), xr = numeric_limits<double>::lowest();
    double yb = numeric_limits<double>::max(), yt = numeric_limits<double>::lowest();

    for (const auto& p : sites) {
        xl = min(xl, p.x);
        xr = max(xr, p.x);
        yb = min(yb, p.y);
        yt = max(yt, p.y);
    }

    xl -= pad; xr += pad;
    yb -= pad; yt += pad;

    Polygon box = {
        {xl, yt}, {xr, yt}, {xr, yb}, {xl, yb}
    };

    vector<Polygon> cells;

    for (int i = 0; i < sites.size(); ++i) {
        Polygon cell = box;
        Point site = sites[i];

        for (int j = 0; j < sites.size(); ++j) {
            if (i == j) continue;

            Edge bisector = perpendicularBisector(site, sites[j]);
            cell = clipPolygon(cell, bisector, site);
        }

        cells.push_back(cell);
    }

    return cells;
}
