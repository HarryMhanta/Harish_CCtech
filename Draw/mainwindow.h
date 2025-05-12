#include<QMainWindow>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *mycentre;
    GLWidget *myopengl;

}