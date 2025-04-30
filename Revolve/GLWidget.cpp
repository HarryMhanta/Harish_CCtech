#include "GLWidget.h"
#include <QtMath>
#include <QMatrix4x4>

RevolveWidget::RevolveWidget(QWidget* parent)
    : QOpenGLWidget(parent) {
    // Add two default control points
    controlPoints << QVector3D(-0.5, -0.5, 0.0);
                //   << QVector3D(0.5, 0.5, 0.0);
}

RevolveWidget::~RevolveWidget() {
    qDebug() << "RevolveWidget destructor called";
}

void RevolveWidget::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&RevolveWidget::update));
    updateTimer->start(16); // ~60 FPS
}

void RevolveWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    aspectRatio = float(w) / float(h ? h : 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float top = tan(fov * 0.5 * M_PI / 180.0f) * nearPlane;
    float right = top * aspectRatio;
    glFrustum(-right, right, -top, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void RevolveWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    drawBezier(); // Draw the 2D Bezier curve
    drawRevolvedSurface(); // Draw the 3D revolved surface
}

QVector3D RevolveWidget::deCasteljau(float t, const QVector<QVector3D>& points) {
    if (points.size() == 1)
        return points[0];

    QVector<QVector3D> next;
    for (int i = 0; i < points.size() - 1; ++i) {
        QVector3D p = (1 - t) * points[i] + t * points[i + 1];
        next.append(p);
    }
    return deCasteljau(t, next);
}

void RevolveWidget::drawBezier() {
    if (controlPoints.size() < 2)
        return;

    // Draw Bezier curve
    glColor3f(1.0f, 1.0f, 1.0f); // White curve
    glBegin(GL_LINE_STRIP);
    const int resolution = 100; 
    for (int i = 0; i <= resolution; ++i) {
        float t = float(i) / resolution;
        QVector3D p = deCasteljau(t, controlPoints);
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();

    // Draw control points
    glPointSize(10.0f);
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glBegin(GL_POINTS);
    for (const auto& p : controlPoints)
        glVertex3f(p.x(), p.y(), p.z());
    glEnd();
}

void RevolveWidget::drawRevolvedSurface() {
    if (controlPoints.size() < 2)
        return;

    const int curveResolution = 100; // Resolution of the Bezier curve
    const int angleResolution = 36; // Number of slices for the revolution (e.g., 10° steps)

    QVector<QVector3D> curvePoints;
    for (int i = 0; i <= curveResolution; ++i) {
        float t = float(i) / curveResolution;
        curvePoints.append(deCasteljau(t, controlPoints));
    }

    glColor3f(0.5f, 0.5f, 1.0f); // Light blue for the surface
    glBegin(GL_QUADS); // Use quads to draw the surface

    for (int i = 0; i < curvePoints.size() - 1; ++i) {
        for (int j = 0; j < angleResolution; ++j) {
            float angle1 = j * (360.0f / angleResolution);
            float angle2 = (j + 1) * (360.0f / angleResolution);

            // Convert angles to radians
            float rad1 = qDegreesToRadians(angle1);
            float rad2 = qDegreesToRadians(angle2);

            // Current and next points on the curve
            QVector3D p1 = curvePoints[i];
            QVector3D p2 = curvePoints[i + 1];

            // Generate vertices for the quad
            QVector3D v1(p1.x() * cos(rad1), p1.y(), p1.x() * sin(rad1));
            QVector3D v2(p1.x() * cos(rad2), p1.y(), p1.x() * sin(rad2));
            QVector3D v3(p2.x() * cos(rad2), p2.y(), p2.x() * sin(rad2));
            QVector3D v4(p2.x() * cos(rad1), p2.y(), p2.x() * sin(rad1));

            // Draw the quad
            glVertex3f(v1.x(), v1.y(), v1.z());
            glVertex3f(v2.x(), v2.y(), v2.z());
            glVertex3f(v3.x(), v3.y(), v3.z());
            glVertex3f(v4.x(), v4.y(), v4.z());
        }
    }

    glEnd();
}

void RevolveWidget::mousePressEvent(QMouseEvent* event) {
    lastMousePos = event->position();

    // Convert mouse position to normalized device coordinates
    float x = (2.0f * lastMousePos.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * lastMousePos.y()) / height();
    QVector3D clickPos(x, y, 0.0f);

    // Check if a control point is clicked
    const float threshold = 0.1f;
    selectedPointIndex = -1; // Reset selected point index
    for (int i = 0; i < controlPoints.size(); ++i) {
        if ((controlPoints[i] - clickPos).length() < threshold) {
            selectedPointIndex = i;
            return;
        }
    }

    // Add a new control point if no point is selected
    if(selectedPointIndex == -1) {
        controlPoints.append(clickPos);
        selectedPointIndex = controlPoints.size() - 1;
    } else {
        // Move the existing control point to the new position
        controlPoints[selectedPointIndex] = clickPos;
    }
    update();
}

void RevolveWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF currentPos = event->position();

    if (event->buttons() & Qt::LeftButton) {
        if (selectedPointIndex != -1) {
            // Update the position of the selected control point
            float x = (2.0f * currentPos.x()) / width() - 1.0f;
            float y = 1.0f - (2.0f * currentPos.y()) / height();
            controlPoints[selectedPointIndex] = QVector3D(x, y, 0.0f);
            update();
        }
    }
    lastMousePos = currentPos;
}

void RevolveWidget::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    selectedPointIndex = -1; // Deselect the control point
}

void RevolveWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close(); // Close the widget on Escape key press
    } else if (event->key() == Qt::Key_R) {
        controlPoints.clear(); // Clear control points on 'R' key press
        update();
    }
}