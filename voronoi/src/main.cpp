#include <QApplication>
#include "voronoiwidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget window;
    VoronoiWidget* vw = new VoronoiWidget(&window);
    vw->setGeometry(10, 10, 600, 600);
    window.resize(620, 620);
    window.show();

    return a.exec();
}
