#ifndef GRAPHICSAPP_H
#define GRAPHICSAPP_H

#include <QOpenGLWidget>
#include <QVector>
#include "primitives/Vertex.h"
#include "primitives/Edge.h"
#include "primitives/Face.h"
#include "Extrusion/Extrusion.h"

class GraphicsApp : public QOpenGLWidget {
    Q_OBJECT

public:
    GraphicsApp(QWidget *parent = nullptr);
    ~GraphicsApp();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<Vertex> vertices;
    QVector<Edge> edges;
    QVector<Face> faces;
    Extrusion extrusion;

    void updateScene();
};

#endif // GRAPHICSAPP_H