#include <QApplication>
#include <QSurfaceFormat>
#include "glwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setVersion(3, 3); // Request OpenGL 3.3
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget widget;
    widget.resize(1000, 800); // Set initial size
    widget.show();

    return app.exec();
}
