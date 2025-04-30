#include "glwidget.h"
#include <QImage>
#include <QFile>
#include <QDir>
#include <cmath>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      sphereVBO(QOpenGLBuffer::VertexBuffer),
      sphereEBO(QOpenGLBuffer::IndexBuffer),
      skyboxVBO(QOpenGLBuffer::VertexBuffer) {
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    loadShaders();
    initSphere();
    initSkybox();

    QStringList faces = {
        "C:\\Migrate\\cubemapSphere\\cubemap\\1.jpeg",
        "C:\\Migrate\\cubemapSphere\\cubemap\\1.jpeg",
        "C:\\Migrate\\cubemapSphere\\cubemap\\1.jpeg",
        "C:\\Migrate\\cubemapSphere\\cubemap\\1.jpeg",
        "C:\\Migrate\\cubemapSphere\\cubemap\\1.jpeg",
        "C:\\Migrate\\cubemapSphere\\cubemap\\1.jpeg"
    };
    cubemapTex = loadCubemap(faces);
}

void GLWidget::resizeGL(int w, int h) {
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / h, 0.1f, 100.0f);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.lookAt(QVector3D(0, 0, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    // Draw Skybox
    glDepthFunc(GL_LEQUAL);
    skyboxProgram.bind();
    int viewLoc = skyboxProgram.uniformLocation("view");
    int projLoc = skyboxProgram.uniformLocation("projection");
    if (viewLoc == -1 || projLoc == -1) {
        qDebug() << "Skybox uniform locations not found!";
    }
    skyboxProgram.setUniformValue(viewLoc, view);
    skyboxProgram.setUniformValue(projLoc, projection);
    glBindVertexArray(skyboxVAO.objectId());
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    skyboxProgram.release();
    glDepthFunc(GL_LESS);

    // Draw Sphere
    sphereProgram.bind();
    QMatrix4x4 model;
    model.translate(0.0f, 0.0f, 0.0f);
    int modelLoc = sphereProgram.uniformLocation("model");
    int sphereViewLoc = sphereProgram.uniformLocation("view");
    int sphereProjLoc = sphereProgram.uniformLocation("projection");
    int cameraPosLoc = sphereProgram.uniformLocation("cameraPos");
    if (modelLoc == -1 || sphereViewLoc == -1 || sphereProjLoc == -1 || cameraPosLoc == -1) {
        qDebug() << "Sphere uniform locations not found!";
    }
    sphereProgram.setUniformValue(modelLoc, model);
    sphereProgram.setUniformValue(sphereViewLoc, view);
    sphereProgram.setUniformValue(sphereProjLoc, projection);
    sphereProgram.setUniformValue(cameraPosLoc, QVector3D(0, 0, 3));
    glBindVertexArray(sphereVAO.objectId());
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    sphereProgram.release();
}

void GLWidget::loadShaders() {
    if (!skyboxProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Migrate\\cubemapSphere\\shaders\\skybox.vert")) {
        qDebug() << "Failed to load skybox vertex shader:" << skyboxProgram.log();
    }
    if (!skyboxProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Migrate\\cubemapSphere\\shaders\\skybox.frag")) {
        qDebug() << "Failed to load skybox fragment shader:" << skyboxProgram.log();
    }
    if (!skyboxProgram.link()) {
        qDebug() << "Failed to link skybox shader program:" << skyboxProgram.log();
    }

    if (!sphereProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Migrate\\cubemapSphere\\shaders\\sphere.vert")) {
        qDebug() << "Failed to load sphere vertex shader:" << sphereProgram.log();
    }
    if (!sphereProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Migrate\\cubemapSphere\\shaders\\sphere.frag")) {
        qDebug() << "Failed to load sphere fragment shader:" << sphereProgram.log();
    }
    if (!sphereProgram.link()) {
        qDebug() << "Failed to link sphere shader program:" << sphereProgram.log();
    }
}

void GLWidget::initSphere() {
    // Initialize the sphere's vertex and index buffers
    QVector<float> vertices;
    QVector<unsigned int> indices;

    const int latitudeBands = 30;
    const int longitudeBands = 30;
    const float radius = 1.0f;

    for (int lat = 0; lat <= latitudeBands; ++lat) {
        float theta = lat * M_PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeBands; ++lon) {
            float phi = lon * 2 * M_PI / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            vertices.append(radius * x);
            vertices.append(radius * y);
            vertices.append(radius * z);
        }
    }

    for (int lat = 0; lat < latitudeBands; ++lat) {
        for (int lon = 0; lon < longitudeBands; ++lon) {
            int first = (lat * (longitudeBands + 1)) + lon;
            int second = first + longitudeBands + 1;

            indices.append(first);
            indices.append(second);
            indices.append(first + 1);

            indices.append(second);
            indices.append(second + 1);
            indices.append(first + 1);
        }
    }

    sphereVBO.create();
    sphereVBO.bind();
    sphereVBO.allocate(vertices.constData(), vertices.size() * sizeof(float));
    sphereVBO.release();

    sphereEBO.create();
    sphereEBO.bind();
    sphereEBO.allocate(indices.constData(), indices.size() * sizeof(unsigned int));
    sphereEBO.release();

    sphereIndexCount = indices.size();
}

void GLWidget::initSkybox() {
    // Initialize the skybox's vertex buffer
    QVector<float> skyboxVertices = {
        // Add skybox cube vertices here
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        // Add remaining cube vertices...
    };

    skyboxVBO.create();
    skyboxVBO.bind();
    skyboxVBO.allocate(skyboxVertices.constData(), skyboxVertices.size() * sizeof(float));
    skyboxVBO.release();
}

GLuint GLWidget::loadCubemap(const QList<QString>& faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (int i = 0; i < faces.size(); ++i) {
        QImage image(faces[i]);
        if (image.isNull()) {
            qDebug() << "Failed to load cubemap texture at" << faces[i];
            continue;
        }
        image = image.convertToFormat(QImage::Format_RGBA8888);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
