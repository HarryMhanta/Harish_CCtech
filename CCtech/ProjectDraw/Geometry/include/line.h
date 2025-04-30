#ifndef LINE_H
#define LINE_H

#include <array>
#include <string>

using namespace std; // Using namespace std here for all standard library components.

template <size_t Dim>
class Line {
private:
    array<double, Dim> start; // Coordinates of the first endpoint
    array<double, Dim> end;   // Coordinates of the second endpoint
    array<double, Dim> direction; // Direction components (differences)

public:
    Line(const array<double, Dim>& start, const array<double, Dim>& end);
    void calculateDirection();
    void generateGnuplotScript(const string& filename) const;
    void plotLine(const string& scriptFilename) const;
};

#endif // LINE_H
