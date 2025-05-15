#include "BezierToolWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <cmath>

BezierToolWidget::BezierToolWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    beziers.append(QVector<QVector3D>()); // Start with first bezier

    // UI Buttons
    finishButton = new QPushButton("Finish", this);
    intersectionButton = new QPushButton("Intersection", this);
    intersectionButton->setEnabled(false);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(finishButton);
    btnLayout->addWidget(intersectionButton);
    btnLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);

    connect(finishButton, &QPushButton::clicked, this, &BezierToolWidget::onFinishClicked);
    connect(intersectionButton, &QPushButton::clicked, this, &BezierToolWidget::onIntersectionClicked);
}

BezierToolWidget::~BezierToolWidget() {}

void BezierToolWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
}

void BezierToolWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void BezierToolWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::blue, 2);
painter.setPen(pen);

// Draw all beziers
for (const auto& controlPoints : beziers) {
    if (controlPoints.size() < 2) continue;

    QPainterPath path;
    path.moveTo(controlPoints[0].toPointF());
    if (controlPoints.size() == 2) {
        path.lineTo(controlPoints[1].toPointF());
    } else if (controlPoints.size() == 3) {
        path.quadTo(controlPoints[1].toPointF(), controlPoints[2].toPointF());
    } else if (controlPoints.size() == 4) {
        path.cubicTo(controlPoints[1].toPointF(), controlPoints[2].toPointF(), controlPoints[3].toPointF());
    } else {
        for (int i = 1; i < controlPoints.size(); ++i)
            path.lineTo(controlPoints[i].toPointF());
    }
    painter.setBrush(Qt::NoBrush); // Add this line to prevent filling
    painter.drawPath(path);

    // Draw control points
    painter.setBrush(Qt::red);
    for (const auto& pt : controlPoints)
        painter.drawEllipse(pt.toPointF(), 4, 4);
}

    // Draw intersection points if any
    if (!intersectionPoints.isEmpty()) {
        painter.setBrush(Qt::green);
        painter.setPen(QPen(Qt::green, 0.2));
        for (const auto& pt : intersectionPoints) {
            painter.drawEllipse(pt.toPointF(), 2, 2);
        }
    }
}

void BezierToolWidget::mousePressEvent(QMouseEvent* event) {
    if (finishedFirst && finishedSecond) return; // No more input

    if (event->button() == Qt::LeftButton) {
        QVector3D pt(event->pos().x(), event->pos().y(), 0);
        beziers[currentBezierIndex].append(pt);
        update();
    }
}

void BezierToolWidget::onFinishClicked() {
    if (!finishedFirst) {
        if (beziers[currentBezierIndex].size() >= 2) {
            finishedFirst = true;
            beziers.append(QVector<QVector3D>());
            currentBezierIndex = 1;
            finishButton->setText("Finish 2");
        }
    } else if (!finishedSecond) {
        if (beziers[currentBezierIndex].size() >= 2) {
            finishedSecond = true;
            finishButton->setEnabled(false);
            intersectionButton->setEnabled(true);
        }
    }
    update();
}

void BezierToolWidget::onIntersectionClicked() {
    intersectionPoints = computeIntersections();
    update();
}

// Helper: Evaluate Bezier at t (supports up to cubic)
QPointF BezierToolWidget::bezierPoint(const QVector<QVector3D>& ctrl, double t) const {
    if (ctrl.size() == 2) {
        return (1 - t) * ctrl[0].toPointF() + t * ctrl[1].toPointF();
    } else if (ctrl.size() == 3) {
        return std::pow(1 - t, 2) * ctrl[0].toPointF()
             + 2 * (1 - t) * t * ctrl[1].toPointF()
             + std::pow(t, 2) * ctrl[2].toPointF();
    } else if (ctrl.size() == 4) {
        return std::pow(1 - t, 3) * ctrl[0].toPointF()
             + 3 * std::pow(1 - t, 2) * t * ctrl[1].toPointF()
             + 3 * (1 - t) * std::pow(t, 2) * ctrl[2].toPointF()
             + std::pow(t, 3) * ctrl[3].toPointF();
    }
    // For more than 4, fallback to polyline
    return ctrl[0].toPointF();
}

// Helper: Compute intersections by sampling points
QVector<QVector3D> BezierToolWidget::computeIntersections() {
    QVector<QVector3D> result;
    if (beziers.size() < 2) return result;
    const auto& b1 = beziers[0];
    const auto& b2 = beziers[1];
    if (b1.size() < 2 || b2.size() < 2) return result;

    // Sample both curves
    QVector<QPointF> curve1, curve2;
    int samples = 200;
    for (int i = 0; i <= samples; ++i) {
        double t = double(i) / samples;
        curve1.append(bezierPoint(b1, t));
        curve2.append(bezierPoint(b2, t));
    }

    // Check for close points (simple intersection detection)
    double threshold = 6.0; // pixels
    for (const QPointF& p1 : curve1) {
        for (const QPointF& p2 : curve2) {
            if (QLineF(p1, p2).length() < threshold) {
                result.append(QVector3D((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2, 0));
            }
        }
    }

    // Remove duplicates
    QVector<QVector3D> unique;
    for (const auto& pt : result) {
        bool found = false;
        for (const auto& u : unique) {
            if ((u - pt).length() < 2.0) { found = true; break; }
        }
        if (!found) unique.append(pt);
    }
    return unique;
}