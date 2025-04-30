#include "robot.h"
#include "fileHandling.h"
#include <memory>
#include<unordered_map>
#include "parent.h"
#include "shapes.h"

void Robot::generateShapes()
{
    
    int choice;

    std::unordered_map<int, std::shared_ptr<Parent<double>>> shapes;
    shapes[1] = std::make_shared<Cube<double>>();
    shapes[2] = std::make_shared<Sphere<double>>(5);
    shapes[3] = std::make_shared<Cylinder<double>>(1, 1); // Example radius and height

while(true)
{
    cout<< "1.Cube\n 2.Sphere\n 3.Cylinder\n 4.Draw Robot\n 5.Exit\n";
    cout <<"Enter you choice: ";
    cin >>choice;
    if(choice==4)
    {
        drawRobo();
        break;
    }
    else if(choice>=1 && choice<=3)
    {
        shapes[choice]->generate();
    }
    else
    {
        cout<<"Invalid choice. Please try again.\n";
    }

}

}

void Robot::drawRobo()
{
   // BuildScript build("Robot", "robot.dat");
   system("gnuplot -e \"set terminal qt; set mouse; set xlabel 'X'; set ylabel 'Y'; set zlabel 'Z'; splot 'robot.dat' using 1:2:3 with lines; pause -1\"");
}







