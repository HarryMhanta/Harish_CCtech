#ifndef FILEHANDLING_H
#define FILEHANDLING_H
 
#include <iostream>
#include <vector>
#include <string>
 
using namespace std;
 
class FileHandling {
 
    public:
        vector<vector<vector<double>>> readDataFile(const string& filename);
        void  writeTransformedData(const string& filename, const vector<vector<vector<double>>>& transformedEntities);
       
};
 
#endif // FILEHANDLING_