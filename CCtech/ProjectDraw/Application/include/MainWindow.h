#pragma once
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
 
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
 
class MainWindow : public QMainWindow {
    Q_OBJECT
 
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
 
private:
    Ui::MainWindow* ui;
    QString selectedShape;  // Store the selected shape name


private slots:
    //void onAddButtonClicked();                // Slot for Add button click
    void onShapeSelected(const QString& shapeName);
    //void onAddShapeClicked();               // Slot for Add Shape button click

};