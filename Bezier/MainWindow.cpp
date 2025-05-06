#include <QApplication>
#include <QSlider>
#include <QVBoxLayout>
#include "bezierGLWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BezierGLWidget widget;
    widget.resize(800, 600);
    widget.show();
    return app.exec();
}



