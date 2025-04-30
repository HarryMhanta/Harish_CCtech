#ifndef GLWIDGET_H
#define GLWIDGET_H
#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>  // Add this include
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void initSphere(float radius, int sectors, int stacks);
    void setupShaders();
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    QPoint lastMousePos;


    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;

    GLuint vao, vbo, ebo;
    int indexCount;

    QMatrix4x4 projection, view;
};

#endif // GLWIDGET_H