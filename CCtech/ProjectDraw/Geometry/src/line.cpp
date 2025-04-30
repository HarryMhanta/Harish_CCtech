#include "line.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std; 

template <size_t Dim>
Line<Dim>::Line(const array<double, Dim>& start, const array<double, Dim>& end)
    : start(start), end(end) {
    calculateDirection();
}

template <size_t Dim>
void Line<Dim>::calculateDirection() {
    for (size_t i = 0; i < Dim; ++i) {
        direction[i] = end[i] - start[i];
    }
}

template <size_t Dim>
void Line<Dim>::generateGnuplotScript(const string& filename) const {
    ofstream gnuplotScript(filename);

    if (!gnuplotScript) {
        cerr << "Error opening file to write gnuplot script." << endl;
        return;
    }

    for (size_t i = 0; i < Dim; ++i) {
        gnuplotScript << "set " << (i == 0 ? "x" : (i == 1 ? "y" : "z"))
                      << "range [" << start[i] - 2 << ":" << end[i] + 2 << "]\n";
    }
    gnuplotScript << "set pointsize 1.5\n";

    if (Dim == 2) {
        gnuplotScript << "plot '-' using 1:2 with lines title '2D Line', \\\n";
        gnuplotScript << "     '-' using 1:2 with points pt 7 ps 1.5 title 'Points'\n";
    } else if (Dim == 3) {
        gnuplotScript << "splot '-' using 1:2:3 with lines title '3D Line', \\\n";
        gnuplotScript << "      '-' using 1:2:3 with points pt 7 ps 1.5 title 'Points'\n";
    }

    for (double t = 0; t <= 1; t += 0.05) {
        for (size_t i = 0; i < Dim; ++i) {
            gnuplotScript << start[i] + t * direction[i] << " ";
        }
        gnuplotScript << "\n";
    }
    gnuplotScript << "e\n";

    for (int i = 0; i < 8; ++i) {
        double t = (i + 1) / 9.0;
        for (size_t j = 0; j < Dim; ++j) {
            gnuplotScript << start[j] + t * direction[j] << " ";
        }
        gnuplotScript << "\n";
    }
    gnuplotScript << "e\n";

    gnuplotScript.close();
}

template <size_t Dim>
void Line<Dim>::plotLine(const string& scriptFilename) const {
    string command = "gnuplot -p " + scriptFilename;
    system(command.c_str());
}

// Explicit instantiation for 2D and 3D
template class Line<2>;
template class Line<3>;
