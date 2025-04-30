#include "voronoiwidget.h"
#include <QPainter>
#include <QOpenGLFunctions>
#include <QDebug>

VoronoiWidget::VoronoiWidget(QWidget* parent) : QOpenGLWidget(parent) {    
    std::vector<Point> sites = {{0, 0}, {10, 0}, {5, 10}};
    double pad = 5.0;
    cells = computeVoronoiCells(sites, pad);
}

VoronoiWidget::~VoronoiWidget() {
    // Destructor logic if needed
}

void VoronoiWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
}

void VoronoiWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 20, -10, 20, -1, 1);  // Adjust bounds for better viewing
    glMatrixMode(GL_MODELVIEW);
}

void VoronoiWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(0.0f, 0.0f, 0.0f); // Black lines

    for (const auto& Polygon : cells) {
        glBegin(GL_LINE_LOOP);
        for (const auto& p : Polygon) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }
}


