#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QVector3D>
#include <QMouseEvent>
#include <QPushButton>

class BezierToolWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit BezierToolWidget(QWidget* parent = nullptr);
    ~BezierToolWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onFinishClicked();
    void onIntersectionClicked();

private:
    QVector<QVector<QVector3D>> beziers; // Each bezier is a vector of control points
    int currentBezierIndex = 0;
    bool finishedFirst = false;
    bool finishedSecond = false;

    QPushButton* finishButton;
    QPushButton* intersectionButton;

    QVector<QVector3D> intersectionPoints;

    // Helper functions
    QVector<QVector3D> computeIntersections();
    QPointF bezierPoint(const QVector<QVector3D>& ctrl, double t) const;
};