#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QVector>
#include <QMouseEvent>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <vector>
#include <QPointF>

struct Polygon {
    QVector<QVector3D> points;          // Points of the polygon
    QVector3D normal;                   // Normal vector of the polygon
    float extrusionHeight = 0.0f;       // Extrusion height for the polygon
    bool isClosed = false;              // Whether the polygon is closed
    QPoint position;                    // Position for free movement
};

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
    QVector<Polygon> polygons;          // List of polygons
    int selectedPolygonIndex = -1;      // Index of the selected polygon
    QPoint lastMousePos;                // Last mouse position for rotation

    float rotationX = 0.0f;             // Rotation around the X-axis
    float rotationY = 0.0f;             // Rotation around the Y-axis
    bool rightMousePressed = false;     // Whether the right mouse button is pressed
    bool polygonClosed = false;         // Whether the current polygon is closed
    float extrusionHeight = 0.0f;       // Extrusion height for the current polygon

    QComboBox *booleanComboBox;         // ComboBox for selecting boolean operations
    QCheckBox *subtractionModeCheckBox; // CheckBox for subtraction mode (A - B or B - A)
    QPushButton *applyBooleanButton;    // Button to apply the selected boolean operation

    // Helper functions
    QVector3D screenToWorld(const QPoint &pos); // Convert screen coordinates to world coordinates
    bool isCloseToFirstPoint(const QVector3D &p, const Polygon &polygon); // Check if a point is close to the first point of a polygon
    QVector3D computePolygonNormal(const QVector<QVector3D> &points); // Compute the normal vector of a polygon

    // Boolean operation functions
    QVector<QVector3D> computeUnion(const Polygon &A, const Polygon &B); // Compute the union of two polygons
    QVector<QVector3D> computeIntersection(const Polygon &A, const Polygon &B); // Compute the intersection of two polygons
    QVector<QVector3D> computeSubtraction(const Polygon &A, const Polygon &B); // Compute the subtraction of two polygons

    // Helper functions for boolean operations
    bool pointInPolygon(const QPointF& point, const std::vector<QPointF>& polygon); // Check if a point is inside a polygon
    bool segmentsIntersect(const QPointF& a1, const QPointF& a2, const QPointF& b1, const QPointF& b2, QPointF& out); // Check if two line segments intersect
    void addAndSortIntersections(std::vector<QPointF>& result, const QPointF& point); // Add and sort intersection points
    std::vector<QPointF> splitEdges(const std::vector<QPointF>& polygon, const std::vector<QPointF>& intersections); // Split edges at intersection points

    // PolygonBoolean operations
    std::vector<QPointF> intersect(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2); // Compute intersection of two polygons
    std::vector<QPointF> unionPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2); // Compute union of two polygons

private slots:
    void onApplyBooleanOperation(); // Slot to handle the application of boolean operations
};