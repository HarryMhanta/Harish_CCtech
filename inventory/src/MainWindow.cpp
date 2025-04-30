// #include "MainWindow.h"
// #include "GLWidget.h" // Your custom OpenGL widget
// #include <QComboBox>
// #include <QPushButton>
// #include <QLabel>
// #include <QListWidget>
// #include <QDoubleSpinBox>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QFrame>
// #include <QMenuBar>
// #include <QStatusBar>
// #include <QMenu>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
// {
//     setWindowTitle("Inventor");
//     resize(1020, 730);

//     // ======= Central Widget & Layout =======
//     QWidget *centralWidget = new QWidget(this);
//     setCentralWidget(centralWidget);

//     QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

//     // ======= OpenGL Widget =======
//     GLWidget *glWidget = new GLWidget(this);
//     glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//     // ======= Right Panel =======
//     QVBoxLayout *rightPanel = new QVBoxLayout();

//     QLabel *shapeLabel = new QLabel("Draw new shapes :", this);
//     QComboBox *shapeComboBox = new QComboBox(this);
//     shapeComboBox->addItems({
//         "Shapes", "Extrusion",
//     });
//     QPushButton *shapeAddButton = new QPushButton("ADD", this);

//     QHBoxLayout *shapeSelectLayout = new QHBoxLayout();
//     shapeSelectLayout->addWidget(shapeComboBox);
//     shapeSelectLayout->addWidget(shapeAddButton);

//     QListWidget *shapeList = new QListWidget(this);

//     QLabel *transformLabel = new QLabel("Apply Transformations :", this);

//     auto createSpinControl = [](const QString &labelText) {
//         QHBoxLayout *layout = new QHBoxLayout();
//         QLabel *label = new QLabel(labelText);
//         QDoubleSpinBox *spin = new QDoubleSpinBox();
//         spin->setMinimum(-1000.0); // Allow negative values
//         spin->setMaximum(1000.0);  // Set a reasonable maximum value
//         spin->setSingleStep(0.1);  // Set the step size for increment/decrement
//         layout->addWidget(label);
//         layout->addWidget(spin);
//         return std::make_pair(layout, spin);
//     };

//     // Transform Controls
//     auto [scaleXLayout, scaleXSpin] = createSpinControl("scale-x :");
//     auto [scaleYLayout, scaleYSpin] = createSpinControl("scale-y :");
//     auto [scaleZLayout, scaleZSpin] = createSpinControl("scale-z :");
//     auto [rotateXLayout, rotateXSpin] = createSpinControl("rotate-x :");
//     auto [rotateYLayout, rotateYSpin] = createSpinControl("rotate-y :");
//     auto [rotateZLayout, rotateZSpin] = createSpinControl("rotate-z :");
//     auto [translateXLayout, translateXSpin] = createSpinControl("translate-x :");
//     auto [translateYLayout, translateYSpin]  = createSpinControl("translate-y :");
//     auto [translateZLayout, translateZSpin] = createSpinControl("translate-z :");

//     QPushButton *transformShapeBtn = new QPushButton("Apply Transformation", this);

//     // Assemble right panel
//     rightPanel->addWidget(shapeLabel);
//     rightPanel->addLayout(shapeSelectLayout);
//     rightPanel->addWidget(shapeList);
//     rightPanel->addWidget(transformLabel);
//     rightPanel->addLayout(scaleXLayout);
//     rightPanel->addLayout(scaleYLayout);
//     rightPanel->addLayout(scaleZLayout);
//     rightPanel->addLayout(rotateXLayout);
//     rightPanel->addLayout(rotateYLayout);
//     rightPanel->addLayout(rotateZLayout);
//     rightPanel->addLayout(translateXLayout);
//     rightPanel->addLayout(translateYLayout);
//     rightPanel->addLayout(translateZLayout);
//     rightPanel->addWidget(transformShapeBtn);
//     rightPanel->addStretch();

//     // ======= Final Layout Assembly =======
//     mainLayout->addWidget(glWidget, 1);
//     mainLayout->addLayout(rightPanel);

//     // ======= Menu Bar =======
//     QMenu *fileMenu = menuBar()->addMenu("File");
//     fileMenu->addAction("Load New File");
//     fileMenu->addAction("Save File");

//     QMenu *editMenu = menuBar()->addMenu("Edit");

//     // ======= Status Bar =======
//     statusBar()->showMessage("Ready");


//     // ======= Connections =======
//     // Store shape selection
//     connect(shapeComboBox, &QComboBox::currentTextChanged, this, [this](const QString& text) {
//         selectedShapeName = text.toStdString();
//         qDebug() << "Shape selected:" << text;
//     });

//     // Handle shape add
//     connect(shapeAddButton, &QPushButton::clicked, this, [this, glWidget]() {

//         if (selectedShapeName == "Shapes") {
//             qDebug("Please select a valid shape.");
//             return;
//         }
        
        
//        if (selectedShapeName == "Extrusion") {
//             qDebug("Extrusion selected");
//             // glWidget -> loadVoronoiDiagram(data, inputs);
            
//         } else {
//             qDebug() << "No geometry data loaded (canceled or invalid).";
//         }
//     });

   
// }
