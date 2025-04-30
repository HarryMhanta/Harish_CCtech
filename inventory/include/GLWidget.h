#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QString>
#include <QPainter>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <cfloat>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QTimer>
#include <iostream>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    QVector<QVector3D> polygonPoints;
    QVector<QVector3D> extrudedVertices;
    bool polygonClosed = false;
    float extrusionHeight = 0.0f;
    QVector3D polygonNormal;

    float rotationX = 0.0f; // Rotation angle around the X-axis
    float rotationY = 0.0f; // Rotation angle around the Y-axis
    bool rightMousePressed = false; // Track if the right mouse button is pressed
    QPoint lastMousePos; // To track the last mouse position for rotation

    QVector3D screenToWorld(const QPoint &pos);
    bool isCloseToFirstPoint(const QVector3D &p);
    void updateExtrusion(float height);
    QVector3D computePolygonNormal(const QVector<QVector3D> &points);
};