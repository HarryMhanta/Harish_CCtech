#include "transformation.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>  
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
typedef vector<vector<double>> Matrix;

Transformation::Transformation(string filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Error opening file: " + filename);
    }

    Matrix entity;
    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            if (!entity.empty()) {
                entities.push_back(entity);  
                entity.clear();
            }
            continue;
        }

        stringstream ss(line);
        vector<double> row;
        double value;

        while (ss >> value) {
            row.push_back(value);
        }
        row.push_back(1);

        if (!row.empty()) {
            entity.push_back(row);
        }
    }

    cout << "Do you want to perform an operation? (yes/no): ";
    cin >> performOperation;

    if (performOperation == "yes" || performOperation == "Yes") {
        cout << "Choose an operation to perform:\n";
        cout << "1. Translate\n";
        cout << "2. Scale\n";
        cout << "3. Rotate\n";
        cin >> operationChoice;

        if (operationChoice == 1) {
            cout << "Enter translation values (tx, ty, tz): ";
            cin >> tx >> ty >> tz;
            for(auto& entity : entities) {
                transformedEntities.push_back(translation(entity, tx, ty, tz));
            }
        } else if (operationChoice == 2) {
            cout << "Enter scaling values (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            for(auto& entity : entities) {
                transformedEntities.push_back(scaling(entity, sx, sy, sz));

            }
        } else if (operationChoice == 3) {
            cout << "Enter rotation angle (in degrees): ";
            cin >> angle;
            cout << "Enter axis of rotation (x, y, z): ";
            cin >> axis;

            for(auto& entity : entities) {
                transformedEntities.push_back(rotation(entity, angle, axis));
            }
        } else {
            cerr << "Invalid operation choice." << endl;
        }
        saveToFile(filename);
    }
}


void Transformation::saveToFile(const string& filename) {
    ofstream pt(filename, ios::app);
    if (!pt) {
        throw runtime_error("Error opening file for writing: " + filename);
    }pt<<"\n";
    pt<<"\n";
    for (const auto& entity : transformedEntities) {
        for (const auto& row : entity) {
            for (size_t i = 0; i < 3; i++) {
                pt << row[i] << " ";
            }
            pt << "\n";
        }
        pt << "\n";
    }
    pt.close();
}

Matrix Transformation::scaling(Matrix& entity, double Sx, double Sy, double Sz) {
    // Define the Scaling Transformation Matrix
    T_matrix = {
        {Sx, 0,  0,  0},
        {0,  Sy, 0,  0},
        {0,  0,  Sz, 0},
        {0,  0,  0,  1}
    };
 
    Matrix new_entity(entity.size(), vector<double>(4, 0));
 
    // Matrix Multiplication: new_entity = T_matrix * entity
    for (size_t i = 0; i < entity.size(); i++) { // Iterate over entity rows
        for (size_t j = 0; j < 4; j++) { // Iterate over transformation columns
            for (size_t k = 0; k < 4; k++) { // Iterate over multiplication
                new_entity[i][j] += entity[i][k] * T_matrix[k][j];
            }
        }
    }
 
    return new_entity;

    
}
 
Matrix Transformation::translation(Matrix& entity, double Tx, double Ty, double Tz) {
    // Define the Translation Transformation Matrix
    T_matrix = {
        {1, 0, 0, Tx},
        {0, 1, 0, Ty},
        {0, 0, 1, Tz},
        {0, 0, 0,  1}
    };
 
    Matrix new_entity(entity.size(), vector<double>(4, 1));
 
    // Matrix Multiplication: new_entity = T_matrix * entity
    for (size_t i = 0; i < entity.size(); i++) { // Iterate over entity rows
        for (size_t j = 0; j < 4; j++) { // Iterate over transformation columns
            new_entity[i][j] = T_matrix[j][3]; // Initialize with translation components
 
            for (size_t k = 0; k < 3; k++) { // Only multiply first three components
                new_entity[i][j] += entity[i][k] * T_matrix[k][j];
            }
        }
    }
 
    return new_entity;
}
 
Matrix Transformation::rotation(Matrix& entity, double angle, char axis) {
    double radians = angle * 3.14 / 180.0; // Convert degrees to radians
 
    // Define Rotation Matrices based on the chosen axis
    if (axis == 'x' || axis == 'X') {
        T_matrix = {
            {1,  0,           0,              0},
            {0,  cos(radians), sin(radians),  0},
            {0,  -sin(radians), cos(radians), 0},
            {0,   0,           0,             1}
        };
    }
    else if (axis == 'y' || axis == 'Y') {
        T_matrix = {
            {cos(radians),  0, -sin(radians), 0},
            {0,            1, 0,              0},
            {sin(radians), 0, cos(radians),   0},
            {0,            0, 0,              1}
        };
    }
    else if (axis == 'z' || axis == 'Z') {
        T_matrix = {
            {cos(radians), sin(radians), 0,   0},
            {-sin(radians), cos(radians),  0, 0},
            {0,           0,            1,    0},
            {0,           0,            0,    1}
        };
    }
    else {
        throw invalid_argument("Invalid axis! Choose 'X', 'Y', or 'Z'.");
    }
 
    Matrix new_entity(entity.size(), vector<double>(4, 0));
 
    // Perform Matrix Multiplication: new_entity = T_matrix * entity
    for (size_t i = 0; i < entity.size(); i++) {  // Iterate over entity rows
        for (size_t j = 0; j < 4; j++) {  // Iterate over transformation columns
            for (size_t k = 0; k < 4; k++) {  // Matrix multiplication
                new_entity[i][j] += entity[i][k] * T_matrix[k][j];
            }
        }
    }

    return new_entity;
}