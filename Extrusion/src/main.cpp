#include <QApplication>
#include "GraphicsApp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    GraphicsApp graphicsApp;
    graphicsApp.show();
    graphicsApp.resize(1000, 800);

    return app.exec();
}