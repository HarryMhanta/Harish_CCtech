#include "fileHandling.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>  
 
using namespace std;
 
// Function to read multiple entities from file
vector<vector<vector<double>>> FileHandling:: readDataFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Error opening file: " + filename);
    }
 
    vector<vector<vector<double>>> entities; // Stores multiple entities
    vector<vector<double>> entity; // Current entity being read
    string line;
 
    while (getline(file, line)) {
        if (line.empty()) {
            if (!entity.empty()) {
                entities.push_back(entity); // Store the completed entity
                entity.clear(); // Reset for the next entity
            }
            continue; // Skip empty line 
        }

        stringstream ss(line);
        vector<double> row;
        double value;
 
        while (ss >> value) {  
            row.push_back(value);
        }
        row.push_back(1); // Add homogeneous coordinate
 
        if (!row.empty()) {
            entity.push_back(row);  
        }
    }
 
    if (!entity.empty()) {
        entities.push_back(entity); // Store last entity if exists
    }
 
    file.close();
    return entities;
}
 
// Function to write transformed entities back to file
void FileHandling :: writeTransformedData(const string& filename, const vector<vector<vector<double>>>& transformedEntities) {
    ofstream file(filename, ios::app); // Open file in append mode
    if (!file) {
        throw runtime_error("Error opening file for writing: " + filename);
    }
 
    file << "\n"; // Add separation before writing
    file << "\n"; // Add separation before writing
 
    for (const auto& entity : transformedEntities) {
        for (const auto& row : entity) {
            for (size_t i = 0; i < row.size() - 1; ++i) { // Exclude the last element
                file << row[i] << " ";
            }
            file << "\n";
        }
        file << "\n"; // Separate entities by a blank line
    }
 
    file.close();
}