#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <vector>
#include <cmath>

// Helper function to generate sphere data
void generateSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    const float PI = 3.14159265359f;

    for (int stack = 0; stack <= stackCount; ++stack)
    {
        float stackAngle = PI / 2 - stack * (PI / stackCount); // From pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);                 // r * cos(u)
        float z = radius * sinf(stackAngle);                  // r * sin(u)

        for (int sector = 0; sector <= sectorCount; ++sector)
        {
            float sectorAngle = sector * (2 * PI / sectorCount); // From 0 to 2pi

            // Vertex position
            float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Texture coordinates
            float s = (float)sector / sectorCount;
            float t = (float)stack / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    // Generate indices
    for (int stack = 0; stack < stackCount; ++stack)
    {
        int k1 = stack * (sectorCount + 1); // Beginning of current stack
        int k2 = k1 + sectorCount + 1;     // Beginning of next stack

        for (int sector = 0; sector < sectorCount; ++sector, ++k1, ++k2)
        {
            if (stack != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (stack != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), shaderProgram(nullptr), sunTexture(nullptr), time(0.0f)
{
    // Timer to update the animation
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        time += 0.016f; // Increment time (approx. 60 FPS)
        update();
    });
    timer->start(16);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete shaderProgram;
    delete sunTexture;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Load and compile shaders
    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Migrate\\smoke\\vertex_shader.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Migrate\\smoke\\fragment_shader.glsl");
    shaderProgram->link();

    // Load texture
    sunTexture = new QOpenGLTexture(QImage("C:\\Migrate\\smoke\\sun.jpg").flipped(Qt::Vertical));
    sunTexture->setMinificationFilter(QOpenGLTexture::Linear);
    sunTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Generate sphere data
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    generateSphere(0.5f, 50, 50, vertices, indices); // Radius = 0.5, 50 sectors, 50 stacks

    // Create VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();

    // Pass time to the shader
    shaderProgram->setUniformValue("time", time);

    // Bind texture
    sunTexture->bind();

    // Render the sphere
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 50 * 50 * 6, GL_UNSIGNED_INT, 0); // 50 sectors, 50 stacks
    glBindVertexArray(0);

    shaderProgram->release();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}