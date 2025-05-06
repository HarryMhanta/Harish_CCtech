#ifndef BEZIERGLWIDGET_H
#define BEZIERGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector>
#include <QMouseEvent>
#include <QTimer>
class BezierGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    BezierGLWidget(QWidget* parent = nullptr);
    ~BezierGLWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QVector<QVector3D> controlPoints;
    QVector3D deCasteljau(float t, const QVector<QVector3D>& points);
    void drawBezier();
    void drawRevolvedSurface();

    int selectedPointIndex = -1; // Index of the selected control point
    QPointF lastMousePos;        // Last mouse position for dragging

    float rotationX = 0.0f;      // Rotation around X-axis
    float rotationY = 0.0f;      // Rotation around Y-axis
    float zoom = -2.0f;          // Zoom level
    float aspectRatio = 1.0f;    // Aspect ratio for the viewport
    float fov = 45.0f;           // Field of view
    float nearPlane = 0.1f;      // Near clipping plane
    float farPlane = 100.0f;     // Far clipping plane

    QTimer* updateTimer;         // Timer for updating the widget
};

#endif // BEZIERGLWIDGET_H