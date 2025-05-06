#include "GLWidget.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QtMath>
#include <GL/gl.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent), zoom(-1.5f) // Slightly farther
{
    
    // Initialize the Shapes
    cube = std::make_shared<Cube<float>>(2.0f, 2.0f, 2.0f); // Example dimensions
    cylinder = std::make_shared<Cylinder<float>>(5.0f, 5.0f); // Example dimensions
    sphere = std::make_shared<Sphere<float>>(5.0f); // Example radius
}



void GLWidget::loadModel(const QString& filePath) {
    vertices.clear();
    indices.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filePath;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("v ")) {
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            if (parts.size() == 4) {
                float x = parts[1].toFloat();
                float y = parts[2].toFloat();
                float z = parts[3].toFloat();
                vertices.append(QVector3D(x, y, z));
            }
        } else if (line.startsWith("f ")) {
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            for (int i = 1; i < parts.size(); ++i) {
                QString part = parts[i];
                QStringList sub = part.split("/");
                int index = sub[0].toInt() - 1;  // OBJ is 1-based
                indices.append(index);
            }
        }
    }

    qDebug() << "Loaded model with" << vertices.size() << "vertices and" << indices.size() << "indices";
    update();
}

void GLWidget::setShape(const QString& shape) {
    currentShape = shape;
    update();  // Trigger repaint
}

void GLWidget::addShape(const QString& shapeName) {
    if (shapeName == "Cube") {
        cube = std::make_shared<Cube<float>>(2.0f, 2.0f, 2.0f); // Create a cube
        qDebug() << "Cube added to the scene.";
    } else if (shapeName == "Sphere") {
        sphere = std::make_shared<Sphere<float>>(2.0f); // Create a sphere
        qDebug() << "Sphere added to the scene.";
    } else if (shapeName == "Cylinder") {
        cylinder = std::make_shared<Cylinder<float>>(2.0f, 5.0f); // Create a cylinder
        qDebug() << "Cylinder added to the scene.";
    } else if (shapeName == "Rectangle") {
        // Add logic for rectangle if applicable
        qDebug() << "Rectangle added to the scene.";
    } else {
        qDebug() << "Unknown shape: " << shapeName;
    }

    currentShape = shapeName; // Update the current shape
    update(); // Trigger a repaint of the OpenGL widget
}

void GLWidget::setCubeDimensions(double length, double breadth, double height) {
    cube = std::make_shared<Cube<float>>(length, breadth, height);
    currentShape = "Cube";
    update(); // Trigger repaint
}

void GLWidget::setSphereRadius(double radius) {
    sphere = std::make_shared<Sphere<float>>(radius);
    currentShape = "Sphere";
    update(); // Trigger repaint
}

void GLWidget::setCylinderDimensions(double radius, double height) {
    cylinder = std::make_shared<Cylinder<float>>(radius, height);
    currentShape = "Cylinder";
    update(); // Trigger repaint
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
 
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
 
    // GLfloat lightAmbient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    // GLfloat lightDiffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    // GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // GLfloat lightPosition[] = { 0.0f, 0.0f, 10.0f, 1.0f };
 
    // glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    // glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    // glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
 
    // GLfloat matSpecular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    // GLfloat matShininess[] = { 5.0f };
    // // glMaterialfv(GL_FRONT, GL_AMBIENT, lightAmbient);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, lightDiffuse);
    // // glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    // glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
 
    // glEnable(GL_NORMALIZE);
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    aspectRatio = float(w) / float(h ? h : 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    float top = tan(fov * 0.5 * M_PI / 180.0f) * nearPlane;
    float right = top * aspectRatio;
    glFrustum(-right, right, -top, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    addAxis(); // Draw the axes

    if (currentShape == "Cube" && cube) {
        loadCubeEdges();
    } else if (currentShape == "Cylinder" && cylinder) {
        loadCylinderEdges();
    }
    else if (currentShape == "Sphere" && sphere) {
        loadSphereEdges();
    }

    // else if (currentShape == "Rectangle" && rectangle) {
    //     //loadRectangleEdges();
    //     qdebug() << "reatangle is not implemented yet";
    // }
    //Add more shapes here
    else if (!vertices.isEmpty() && !indices.isEmpty()) {
        glColor3f(1.0f, 1.0f, 0.0f);  // Yellow wireframe
    
        glBegin(GL_LINES);
        for (int i = 0; i + 2 < indices.size(); i += 3) {
            QVector3D v1 = vertices[indices[i]];
            QVector3D v2 = vertices[indices[i + 1]];
            QVector3D v3 = vertices[indices[i + 2]];
    
            glVertex3f(v1.x(), v1.y(), v1.z());
            glVertex3f(v2.x(), v2.y(), v2.z());
    
            glVertex3f(v2.x(), v2.y(), v2.z());
            glVertex3f(v3.x(), v3.y(), v3.z());
    
            glVertex3f(v3.x(), v3.y(), v3.z());
            glVertex3f(v1.x(), v1.y(), v1.z());
        }
        glEnd();
    }
    
    
}

void GLWidget::addAxis()
{
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); // X-axis in red
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f); // Y-axis in green
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); // Z-axis in blue
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();
}

void GLWidget::loadCubeEdges() {
    if (!cube) return;

    // Get the edges of the cube
    const auto& edges = cube->getEdges(); // Directly fetch QVector3D pairs

    glColor3f(1.0f, 0.0f, 0.0f); // Red color for edges

    glBegin(GL_LINES);
    for (const auto& edge : edges) {
        glVertex3f(edge.first.x(), edge.first.y(), edge.first.z());
        glVertex3f(edge.second.x(), edge.second.y(), edge.second.z());
    }
    glEnd();
}

void GLWidget::loadSphereEdges() {
    if (!sphere) return;

    // Get the edges of the sphere
    const std::vector<std::pair<QVector3D, QVector3D>>& edges = sphere->getEdges();

    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for edges

    glBegin(GL_LINES);
    for (const auto& edge : edges) {
        const QVector3D& vertex1 = edge.first;
        const QVector3D& vertex2 = edge.second;

        glVertex3f(vertex1.x(), vertex1.y(), vertex1.z());
        glVertex3f(vertex2.x(), vertex2.y(), vertex2.z());
    }
    glEnd();
}

// void GLWidget::loadRectangleEdges(){
//     if (!rectangle) return;

//     // Get the edges of the rectangle
//     const std::vector<std::pair<QVector3D, QVector3D>>& edges = rectangle->getEdges();

//     glColor3f(1.0f, 0.0f, 1.0f); // Magenta color for edges

//     glBegin(GL_LINES);
//     for (const auto& edge : edges) {
//         const QVector3D& vertex1 = edge.first;
//         const QVector3D& vertex2 = edge.second;

//         glVertex3f(vertex1.x(), vertex1.y(), vertex1.z());
//         glVertex3f(vertex2.x(), vertex2.y(), vertex2.z());
//     }
//     glEnd();
// }

void GLWidget::loadCylinderEdges() {
    if (!cylinder) return;

    // Get the edges of the cylinder
    const std::vector<std::pair<QVector3D, QVector3D>>& edges = cylinder->getEdges();

    glColor3f(0.0f, 1.0f, 0.0f); // Green color for edges

    glBegin(GL_LINES);
    for (const auto& edge : edges) {
        const QVector3D& vertex1 = edge.first;
        const QVector3D& vertex2 = edge.second;

        glVertex3f(vertex1.x(), vertex1.y(), vertex1.z());
        glVertex3f(vertex2.x(), vertex2.y(), vertex2.z());
    }
    glEnd();
}


void GLWidget::mousePressEvent(QMouseEvent* event) {
    lastMousePos = event->position();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF currentPos = event->position();
    int dx = currentPos.x() - lastMousePos.x();
    int dy = currentPos.y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy * 0.5f;
        rotationY += dx * 0.5f;
        update();
    }
    lastMousePos = currentPos;
}

void GLWidget::wheelEvent(QWheelEvent* event) {
    zoom += event->angleDelta().y() / 600.0f; // smoother zoom
    update();
}
