#pragma once
#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GLWidget;  // Forward declaration for GLWidget

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    QString selectedShape;  // Store the selected shape name
    QPushButton *extrudeButton;
    QPushButton *bezierToolButton;

    // Member variables for UI components 
    GLWidget* openGLWidget;
    QPushButton* saveButton;
    QPushButton* loadOBJButton;
    QPushButton* loadSTLButton;
    QComboBox* comboBox;
    QPushButton* addButton;
    QGroupBox* groupBox;
    QSpinBox* spinBox;

private slots:
    void onShapeSelected(const QString& shapeName);  // Handle shape selection
    void onAddButtonClicked();                      // Handle Add button click
    void onSaveButtonClicked();                     // Handle Save button click
    void onLoadOBJButtonClicked();                  // Handle Load OBJ button click
    void onLoadSTLButtonClicked();                  // Handle Load STL button click
    void onExtrudeButtonClicked();                // Handle Extrude button click
    void onBezierToolButtonClicked();               // Handle Bezier Tool button click
};