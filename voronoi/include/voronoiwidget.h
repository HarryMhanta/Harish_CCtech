// voronoiwidget.h
#pragma once
#ifndef VORONOIWIDGET_H
#define VORONOIWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "voronoi.h" // Put your existing Voronoi code here

class VoronoiWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit VoronoiWidget(QWidget* parent = nullptr);
    ~VoronoiWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    std::vector<Polygon> cells;
};

#endif // VORONOIWIDGET_H