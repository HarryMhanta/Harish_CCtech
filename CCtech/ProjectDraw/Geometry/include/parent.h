#ifndef PARENT_H
#define PARENT_H
#include <memory>
#include <iostream>

using namespace std;

template <typename T>
class Parent{
    public:
        Parent()
        {
            cout << "Parent Constructor invoked"<<endl;
        }

        
    virtual void saveToFile() = 0;
    virtual std::shared_ptr<Parent<T>> generate() = 0;


        ~Parent()
        {
        cout<<"Parent destructor invoked"<<endl;
        }

};
#endif