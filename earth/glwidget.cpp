#include "glwidget.h"
#include <QMatrix4x4>
#include <QImage>
#include <QMouseEvent>  // Added for QMouseEvent
#include <cmath>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), texture(nullptr) {}
GLWidget::~GLWidget() {
    makeCurrent();
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    delete texture;
    doneCurrent();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    initSphere(1.0f, 72, 36);
    setupShaders();

    texture = new QOpenGLTexture(QImage("C:\\Migrate\\earth\\earth2.jpg").flipped(Qt::Vertical));  // Fixed deprecated warning

    view.lookAt(QVector3D(0, 0, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
}

void GLWidget::resizeGL(int w, int h) {
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / h, 0.1f, 100.0f);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();
    texture->bind();

    QMatrix4x4 model;
    model.rotate(rotationX, 1.0f, 0.0f, 0.0f);
    model.rotate(rotationY, 0.0f, 1.0f, 0.0f);

    program.setUniformValue("model", model);
    program.setUniformValue("view", view);
    program.setUniformValue("projection", projection);
    program.setUniformValue("lightDir", QVector3D(-0.5f, -1.0f, -0.3f));
    program.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    program.setUniformValue("viewPos", QVector3D(0, 0, 3));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    program.release();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->position().x() - lastMousePos.x();
    int dy = event->position().y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy;
        rotationY += dx;
        update();
    }

    lastMousePos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    // Optional: Add functionality if needed
}

void GLWidget::initSphere(float radius, int sectors, int stacks) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * M_PI / sectors;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float s = (float)j / sectors;
            float t = (float)i / stacks;

            vertices.insert(vertices.end(), {x, y, z, x, y, z, s, t});
        }
    }

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.insert(indices.end(), {static_cast<unsigned int>(k1), static_cast<unsigned int>(k2), static_cast<unsigned int>(k1 + 1)});
            }
            if (i != (stacks - 1)) {
                indices.insert(indices.end(), {static_cast<unsigned int>(k1 + 1), static_cast<unsigned int>(k2), static_cast<unsigned int>(k2 + 1)});
            }
        }
    }

    indexCount = indices.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void GLWidget::setupShaders() {
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTex;
    gl_Position = projection * view * vec4(FragPos, 1.0);
})");

    program.addShaderFromSourceCode(QOpenGLShader::Fragment, R"(#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 light = normalize(-lightDir);
    float diff = max(dot(norm, light), 0.0);

    vec3 color = texture(texture1, TexCoord).rgb;
    vec3 ambient = 0.2 * color;
    vec3 diffuse = diff * color * lightColor;

    FragColor = vec4(ambient + diffuse, 1.0);
})");

    program.link();
}
