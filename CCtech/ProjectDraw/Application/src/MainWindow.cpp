#include "MainWindow.h"
#include "GLWidget.h"
#include "PolygonExtrusionWidget.h"
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug> // For debugging purposes
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1274, 891);
    setWindowTitle("MainWindow");

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);


    // OpenGL Widget
    openGLWidget = new GLWidget(centralWidget);

    // Save Button
    saveButton = new QPushButton("Savefile", centralWidget);

    // Load OBJ Button
    loadOBJButton = new QPushButton("Load OBJ", centralWidget);

    // Load STL Button
    loadSTLButton = new QPushButton("Load STL", centralWidget);

    // ComboBox for Shapes
    comboBox = new QComboBox(centralWidget);
    comboBox->addItem("Shapes");
    comboBox->addItem("Rectangle");
    comboBox->addItem("Cube");
    comboBox->addItem("Cylinder");
    comboBox->addItem("Sphere");

    // Add Button
    addButton = new QPushButton("Add", centralWidget);

    // Transformations GroupBox
    groupBox = new QGroupBox("Transformations", centralWidget);
    spinBox = new QSpinBox(groupBox);

    // Layout for Transformations GroupBox
    QVBoxLayout *groupBoxLayout = new QVBoxLayout(groupBox);
    groupBoxLayout->addWidget(spinBox);
    groupBox->setLayout(groupBoxLayout);

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Top Layout for ComboBox and Add Button
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(comboBox);
    comboBox->setMinimumWidth(50); // Set minimum width for ComboBox
    topLayout->addWidget(addButton);
    addButton->setMinimumWidth(50); // Set minimum width for Add Button

    // Bottom Layout for Buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(saveButton);
    bottomLayout->addWidget(loadSTLButton);
    bottomLayout->addWidget(loadOBJButton);

    //bottom layout for extrude button
    extrudeButton = new QPushButton("Extrude", centralWidget);
    bottomLayout->addWidget(extrudeButton);

    // Add widgets to the main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(openGLWidget, 1); // Stretch factor to make it resizable
    mainLayout->addWidget(groupBox);
    mainLayout->addLayout(bottomLayout);

    centralWidget->setLayout(mainLayout);

    // Menu Bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *menuSketcher = new QMenu("Sketcher", menuBar);
    menuBar->addMenu(menuSketcher);

    // Status Bar
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    // Connect signals to slots
    connect(comboBox, &QComboBox::currentTextChanged, this, &MainWindow::onShapeSelected);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(loadOBJButton, &QPushButton::clicked, this, &MainWindow::onLoadOBJButtonClicked);
    connect(loadSTLButton, &QPushButton::clicked, this, &MainWindow::onLoadSTLButtonClicked);
    connect(extrudeButton, &QPushButton::clicked, this, &MainWindow::onExtrudeButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onShapeSelected(const QString& shapeName)
{
    selectedShape = shapeName; // Update the selected shape
    qDebug() << "Shape selected:" << selectedShape; // Log the selected shape

    if (shapeName == "Cube") {
        bool ok;
        double length = QInputDialog::getDouble(this, "Cube Dimensions", "Enter length:", 2.0, 0.1, 100.0, 2, &ok);
        if (!ok) return;

        double breadth = QInputDialog::getDouble(this, "Cube Dimensions", "Enter breadth:", 2.0, 0.1, 100.0, 2, &ok);
        if (!ok) return;

        double height = QInputDialog::getDouble(this, "Cube Dimensions", "Enter height:", 2.0, 0.1, 100.0, 2, &ok);
        if (!ok) return;

        // Pass the dimensions to the OpenGL widget
        openGLWidget->setCubeDimensions(length, breadth, height);
    } else if (shapeName == "Sphere") {
        bool ok;
        double radius = QInputDialog::getDouble(this, "Sphere Dimensions", "Enter radius:", 2.0, 0.1, 100.0, 2, &ok);
        if (!ok) return;

        // Pass the radius to the OpenGL widget
        openGLWidget->setSphereRadius(radius);
    } else if (shapeName == "Cylinder") {
        bool ok;
        double radius = QInputDialog::getDouble(this, "Cylinder Dimensions", "Enter radius:", 2.0, 0.1, 100.0, 2, &ok);
        if (!ok) return;

        double height = QInputDialog::getDouble(this, "Cylinder Dimensions", "Enter height:", 5.0, 0.1, 100.0, 2, &ok);
        if (!ok) return;

        // Pass the dimensions to the OpenGL widget
        openGLWidget->setCylinderDimensions(radius, height);
    }
}

void MainWindow::onExtrudeButtonClicked() {
    PolygonExtrusionWidget *extrusionWidget = new PolygonExtrusionWidget();
    extrusionWidget->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    extrusionWidget->setWindowTitle("Polygon Extrusion");
    extrusionWidget->resize(800, 600);
    extrusionWidget->show();
}

void MainWindow::onAddButtonClicked()
{
    if (selectedShape.isEmpty() || selectedShape == "Shapes") {
        QMessageBox::warning(this, "Warning", "Please select a valid shape before adding.");
        return;
    }
    openGLWidget->addShape(selectedShape); // Add the selected shape to the OpenGL widget
    qDebug() << "Added shape:" << selectedShape;
}

void MainWindow::onSaveButtonClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", "", "OBJ Files (*.obj);;All Files (*)");
    if (!filePath.isEmpty()) {
        // Implement save logic in GLWidget
        qDebug() << "Save file to:" << filePath;
    }
}

void MainWindow::onLoadOBJButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load OBJ File", "", "OBJ Files (*.obj);;All Files (*)");
    if (!filePath.isEmpty()) {
        openGLWidget->loadModel(filePath); // Load the OBJ file in the OpenGL widget
        qDebug() << "Loaded OBJ file:" << filePath;
    }
}

void MainWindow::onLoadSTLButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load STL File", "", "STL Files (*.stl);;All Files (*)");
    if (!filePath.isEmpty()) {
        // Implement STL loading logic in GLWidget
        qDebug() << "Loaded STL file:" << filePath;
    }
}
