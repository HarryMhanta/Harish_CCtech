#include <QApplication>
#include "MainWindow.h"
 
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
 
    MainWindow window;
    window.show();
    window.resize(1200, 900); // Set the initial size of the window

    return app.exec();
}