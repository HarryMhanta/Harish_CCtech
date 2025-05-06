#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QVector>
#include <QMouseEvent>

class PolygonExtrusionWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit PolygonExtrusionWidget(QWidget *parent = nullptr);
    ~PolygonExtrusionWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<QVector3D> polygonPoints;
    QVector3D polygonNormal;
    float extrusionHeight = 0.0f;
    bool polygonClosed = false;
    bool rightMousePressed = false;
    QPoint lastMousePos;
    float rotationX = 0.0f;
    float rotationY = 0.0f;

    QVector3D screenToWorld(const QPoint &pos);
    bool isCloseToFirstPoint(const QVector3D &p);
    QVector3D computePolygonNormal(const QVector<QVector3D> &points);
};