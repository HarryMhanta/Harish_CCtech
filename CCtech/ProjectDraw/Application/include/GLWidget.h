#pragma once
 
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QString>
#include "Cube.h"
#include "Sphere.h"
#include <QVector3D>
#include <QVector>
#include "cylinder.h"
#include <QMouseEvent>
#include <QTimer>
 
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
 
public:
    // Other declarations...
    QString currentShape; // Track selected shape
    //QVector<QVector3D> bezierControlPoints;

    void setShape(const QString& shape); // Setter to update shape
    void loadSphereEdges(); // Add this line to declare the method
    void loadCubeEdges(); // Add this line to declare the method
    void loadCylinderEdges(); // Add this line to declare the method
    //void loadRectangleEdges(); // Add this line to declare the method
    explicit GLWidget(QWidget* parent = nullptr);
    void loadModel(const QString& filepath); // Your custom model loader
    void addShape(const QString& shapeName);  // e.g., "Cube", "Sphere", etc.
    //void myInit();
    void addAxis(); // Add this line to declare the method
    //void drawBezier();
    //QVector3D deCasteljau(float t, const QVector<QVector3D>& points);
    void setCubeDimensions(double length, double breadth, double height);
    void setSphereRadius(double radius);
    void setCylinderDimensions(double radius, double height);
    

 
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent (QWheelEvent* event) override;
 
private:
    QVector<QVector3D> vertices;
    QVector<unsigned int> indices;
    QVector<QVector3D> normals;
    std::shared_ptr<Cube<float>> cube;  // Use shared pointer for Cube
    std::shared_ptr<Cylinder<float>> cylinder;  // Use shared pointer for Cylinder
    std::shared_ptr<Sphere<float>> sphere;  // Use shared pointer for Sphere

 
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    QPointF lastMousePos;
 
    QTimer* updateTimer;
 
    float zoom = -2.0f; // Distance from camera
    float aspectRatio = 1.0f; // Aspect ratio for the viewport
    float fov = 45.0f; // Field of view
    float nearPlane = 0.1f; // Near clipping plane
    float farPlane = 100.0f; // Far clipping plane
    float cameraSpeed = 0.1f; // Speed of camera movement
    float cameraSensitivity = 0.1f; // sensitivity for mouse movement
    float cameraRotationSpeed = 0.1f; // speed of camera rotation
    float cameraZoomSpeed = 0.1f; // speed of camera zooming
    float cameraPanSpeed = 0.1f; // speed of camera panning
    float cameraTiltSpeed = 0.1f; // speed of camera tilting
    float cameraRollSpeed = 0.1f; // speed of camera rolling
    float cameraDollySpeed = 0.1f; // speed of camera dolly
    float cameraTrackSpeed = 0.1f; // speed of camera tracking
    float cameraOrbitSpeed = 0.1f; // speed of camera orbiting
    float cameraPanSensitivity = 0.1f; // sensitivity for camera panning
    float cameraTiltSensitivity = 0.1f; // sensitivity for camera tilting
    float cameraRollSensitivity = 0.1f; // sensitivity for camera rolling
    float cameraDollySensitivity = 0.1f; // sensitivity for camera dolly
    float cameraTrackSensitivity = 0.1f; // sensitivity for camera tracking
    float cameraOrbitSensitivity = 0.1f; // sensitivity for camera orbiting
 
};