#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <string>
#include <vector>
 
using namespace std;


 
// [T] = | a b c p | // row * col
//       | d e f q |
//       | g i j r |
//       | l m n s |
class Transformation {
    public:
        Transformation(string filename);
        void saveToFile(const string& filename);
        
        vector<vector<double>> scaling(vector<vector<double>>& entity, double sx, double sy, double sz);
        vector<vector<double>> translation(vector<vector<double>>& entity, double tx, double ty, double tz);
        vector<vector<double>> rotation(vector<vector<double>>& entity, double angle, char axis);

    
    private:
        vector<vector<vector<double>>> entities;
        vector<vector<vector<double>>> transformedEntities;
        vector<vector<double>> T_matrix;

        
        string performOperation;
        int operationChoice;
        double tx, ty, tz;
        double sx, sy, sz;
        double angle;
        char axis;
    };
    

#endif