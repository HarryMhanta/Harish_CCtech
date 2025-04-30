#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void initSphere();
    void initSkybox();
    GLuint loadCubemap(const QStringList &faces);
    void loadShaders();

    QOpenGLShaderProgram sphereProgram;
    QOpenGLShaderProgram skyboxProgram;

    QOpenGLVertexArrayObject sphereVAO;
    QOpenGLBuffer sphereVBO, sphereEBO;
    int sphereIndexCount;

    QOpenGLVertexArrayObject skyboxVAO;
    QOpenGLBuffer skyboxVBO;

    GLuint cubemapTex;

    QMatrix4x4 projection;
};
