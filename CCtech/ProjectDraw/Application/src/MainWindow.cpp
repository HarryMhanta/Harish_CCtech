#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // GLWidget *glWidget = new GLWidget(this);
    // glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(ui->pushButton, &QPushButton::clicked, this, []() {
        qDebug("Savefile clicked!");
    });

    // Connect Load OBJ button
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open OBJ or STL File", "", "Model Files (*.obj *.stl)");
    
        if (!filePath.isEmpty()) {
            if (filePath.endsWith(".obj", Qt::CaseInsensitive)) {
                ui->openGLWidget->loadModel(filePath);  // Load the OBJ model into GLWidget
            } else if (filePath.endsWith(".stl", Qt::CaseInsensitive)) {
                ui->openGLWidget->loadModel(filePath);  // Load the STL model into GLWidget
            }
        }
    });

    // connect Load STL button
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){
        QString filePath = QFileDialog::getOpenFileName(this, "Open STL File", "", "STL Files (*.stl)");
    
        if (!filePath.isEmpty()) {
            ui->openGLWidget->loadModel(filePath);  // Load the STL model into GLWidget
        }
    });

    // Connect Shape ComboBox selection
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::onShapeSelected);
}

    void MainWindow::onShapeSelected(const QString& shapeName) {
        selectedShape = shapeName;
        GLWidget* glWidget = ui->openGLWidget;
        glWidget->setShape(shapeName);
    }
    
MainWindow::~MainWindow()
{
    delete ui;
}
