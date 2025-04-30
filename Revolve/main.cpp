#include <QApplication>
#include <QSlider>
#include <QVBoxLayout>
#include "GLWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RevolveWidget widget;
    widget.resize(800, 600);
    widget.show();
    return app.exec();
}



