#include "GLWidget.h"
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <cmath>
 
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
 
        setMouseTracking(true);
}
 
GLWidget::~GLWidget() {}
 
void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}
 
void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}
 
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
 
    // Apply rotation
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto &p : polygonPoints)
        glVertex3f(p.x(), p.y(), p.z());
    glEnd();
 
    if (polygonClosed) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < polygonPoints.size(); ++i) {
            int next = (i + 1) % polygonPoints.size();
            QVector3D base1 = polygonPoints[i];
            QVector3D base2 = polygonPoints[next];
            QVector3D top1 = base1 + polygonNormal * extrusionHeight;
            QVector3D top2 = base2 + polygonNormal * extrusionHeight;
 
            glVertex3f(base1.x(), base1.y(), base1.z());
            glVertex3f(top1.x(), top1.y(), top1.z());
 
            glVertex3f(top1.x(), top1.y(), top1.z());
            glVertex3f(top2.x(), top2.y(), top2.z());
 
            glVertex3f(top2.x(), top2.y(), top2.z());
            glVertex3f(base2.x(), base2.y(), base2.z());
 
            glVertex3f(base2.x(), base2.y(), base2.z());
            glVertex3f(base1.x(), base1.y(), base1.z());
        }
        glEnd();
    }
 
    if (!polygonPoints.isEmpty()) {
        QVector3D first = polygonPoints.first();
        glPointSize(8.0f);
        glColor3f(0.0f, 0.0f, 1.0f); // blue
        glBegin(GL_POINTS);
        glVertex3f(first.x(), first.y(), first.z());
        glEnd();
    }
 
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonDblClick) {
        rightMousePressed = true; // Enable rotation
        lastMousePos = event->pos(); // Store the last mouse position
        qDebug() << "Right mouse pressed:" << rightMousePressed;
    } else if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonDblClick) {
        QVector3D worldPoint = screenToWorld(event->pos());
 
        if (!polygonClosed) {
            if (!polygonPoints.isEmpty() && isCloseToFirstPoint(worldPoint)) {
                polygonClosed = true;
                polygonNormal = computePolygonNormal(polygonPoints);
            } else {
                polygonPoints.append(worldPoint);
            }
            update();
        }
    }
}
 
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (rightMousePressed) {
        // Calculate the rotation based on mouse movement
        float dx = event->pos().x() - lastMousePos.x();
        float dy = event->pos().y() - lastMousePos.y();
 
        rotationX += dy * 0.5f; // Adjust sensitivity as needed 
        rotationY += dx * 0.5f; // Adjust sensitivity as needed
 
        lastMousePos = event->pos(); // Update the last mouse position
        update(); // Request re-render
    } else if (polygonClosed && (event->buttons() & Qt::LeftButton)) {
        float dy = event->pos().y() - height() / 2.0f;
        extrusionHeight = dy * 0.01f; // Scale extrusion
        qDebug() << "Height:" << extrusionHeight;
        update();
    }
}
 
void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = false; // Disable rotation
    }
}
 
QVector3D GLWidget::screenToWorld(const QPoint &pos) {
    float x = (2.0f * pos.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * pos.y()) / height();
    return QVector3D(x, y, 0.0f); // 2D plane
}
 
bool GLWidget::isCloseToFirstPoint(const QVector3D &p) {
    if (polygonPoints.isEmpty()) return false;
    return (polygonPoints.first() - p).length() < 0.05f;
}
 
QVector3D GLWidget::computePolygonNormal(const QVector<QVector3D> &points) {
    QVector3D normal(0, 0, 0);
    for (int i = 0; i < points.size(); ++i) {
        const QVector3D &curr = points[i];
        const QVector3D &next = points[(i + 1) % points.size()];
        normal += QVector3D::crossProduct(curr, next);
    }
    return normal.normalized();
}