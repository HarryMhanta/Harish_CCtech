#include <QApplication>
#include "MainWindow.h"
 
int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Create the application instance
 
    MainWindow window; //object of MainWindow class
    window.show(); // Show the main window
    window.resize(1200, 900); // Set the initial size of the window

    return app.exec();
}