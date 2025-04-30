#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDialog>
#include "GLWidget.h"
// #include "MainWindow.h"
 
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    GLWidget widget;
    widget.resize(1000, 800);
    widget.show();


    return app.exec();
}