#include "PolygonExtrusionWidget.h"
#include <QOpenGLFunctions>
#include <QDebug>
#include <cmath>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>

PolygonExtrusionWidget::PolygonExtrusionWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
    setMouseTracking(true);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    booleanComboBox = new QComboBox(this);
    booleanComboBox->addItem("Union");
    booleanComboBox->addItem("Subtraction");
    booleanComboBox->addItem("Intersection");
    booleanComboBox->setFixedSize(100, 25);

    subtractionModeCheckBox = new QCheckBox("A - B", this); // Toggle
    subtractionModeCheckBox->setChecked(true);
    subtractionModeCheckBox->setFixedSize(100, 20);

    applyBooleanButton = new QPushButton("Apply", this);
    applyBooleanButton->setFixedSize(100, 25);
    connect(applyBooleanButton, &QPushButton::clicked, this, &PolygonExtrusionWidget::onApplyBooleanOperation);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(booleanComboBox);
    layout->addWidget(subtractionModeCheckBox);
    layout->addWidget(applyBooleanButton);
    layout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    layout->setContentsMargins(10, 10, 10, 10); 
    setLayout(layout);
}

PolygonExtrusionWidget::~PolygonExtrusionWidget() {}

void PolygonExtrusionWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void PolygonExtrusionWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void PolygonExtrusionWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (const auto &polygon : polygons) {
        glColor3f(1.5f, 0.5f, 0.5f);
        glBegin(GL_LINE_STRIP);
        for (const auto &p : polygon.points) {
            glVertex3f(p.x(), p.y(), p.z());
        }
        if (polygon.isClosed) {
            glVertex3f(polygon.points.first().x(), polygon.points.first().y(), polygon.points.first().z());
        }
        glEnd();

        if (!polygon.points.isEmpty()) {
            QVector3D first = polygon.points.first();
            glPointSize(8.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_POINTS);
            glVertex3f(first.x(), first.y(), first.z());
            glEnd();
        }

        if (polygon.isClosed) {
            glColor3f(1.5f, 0.5f, 0.5f);
            glBegin(GL_LINES);
            for (int i = 0; i < polygon.points.size(); ++i) {
                int next = (i + 1) % polygon.points.size();
                QVector3D base1 = polygon.points[i];
                QVector3D base2 = polygon.points[next];
                QVector3D top1 = base1 + polygon.normal * extrusionHeight;
                QVector3D top2 = base2 + polygon.normal * extrusionHeight;

                glVertex3f(base1.x(), base1.y(), base1.z());
                glVertex3f(top1.x(), top1.y(), top1.z());

                glVertex3f(top1.x(), top1.y(), top1.z());
                glVertex3f(top2.x(), top2.y(), top2.z());

                glVertex3f(top2.x(), top2.y(), top2.z());
                glVertex3f(base2.x(), base2.y(), base2.z());
            }
            glEnd();
        }
    }
}

void PolygonExtrusionWidget::mousePressEvent(QMouseEvent *event) {
    QVector3D worldPoint = screenToWorld(event->pos());

    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonDblClick) {
        if (polygonClosed || selectedPolygonIndex == -1 || polygons.isEmpty()) {
            Polygon newPolygon;
            newPolygon.points.append(worldPoint);
            polygons.append(newPolygon);
            selectedPolygonIndex = polygons.size() - 1;
            polygonClosed = false;
            qDebug() << "Started a new polygon. Index:" << selectedPolygonIndex;
        } else {
            if (!polygons[selectedPolygonIndex].points.isEmpty() &&
                isCloseToFirstPoint(worldPoint, polygons[selectedPolygonIndex])) {
                polygons[selectedPolygonIndex].isClosed = true;
                polygons[selectedPolygonIndex].normal = computePolygonNormal(polygons[selectedPolygonIndex].points);
                polygonClosed = true;
                qDebug() << "Polygon closed. Index:" << selectedPolygonIndex;
            } else {
                polygons[selectedPolygonIndex].points.append(worldPoint);
                qDebug() << "Added point to polygon. Index:" << selectedPolygonIndex;
            }
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        rightMousePressed = true;
        lastMousePos = event->pos();
        qDebug() << "Right mouse pressed:" << rightMousePressed;
    }
}

void PolygonExtrusionWidget::mouseMoveEvent(QMouseEvent *event) {
    if (rightMousePressed) {
        float dx = event->pos().x() - lastMousePos.x();
        float dy = event->pos().y() - lastMousePos.y();

        rotationX += dy * 0.5f;
        rotationY += dx * 0.5f;

        lastMousePos = event->pos();
        update();
    } else if (polygonClosed && (event->buttons() & Qt::LeftButton)) {
        float dy = event->pos().y() - height() / 2.0f;
        extrusionHeight = dy * 0.01f;
        qDebug() << "Height:" << extrusionHeight;
        update();
    }
}

void PolygonExtrusionWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = false;
    }
}

QVector3D PolygonExtrusionWidget::screenToWorld(const QPoint &pos) {
    float x = (2.0f * pos.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * pos.y()) / height();
    return QVector3D(x, y, 0.0f);
}

bool PolygonExtrusionWidget::isCloseToFirstPoint(const QVector3D &p, const Polygon &polygon) {
    if (polygon.points.isEmpty()) return false;
    return (polygon.points.first() - p).length() < 0.05f;
}

QVector3D PolygonExtrusionWidget::computePolygonNormal(const QVector<QVector3D> &points) {
    QVector3D normal(0, 0, 0);
    for (int i = 0; i < points.size(); ++i) {
        const QVector3D &curr = points[i];
        const QVector3D &next = points[(i + 1) % points.size()];
        normal += QVector3D::crossProduct(curr, next);
    }
    return normal.normalized();
}

bool PolygonExtrusionWidget::isPointInsidePolygon(const QVector3D &point, const Polygon &polygon) {
    int count = 0;
    for (int i = 0; i < polygon.points.size(); ++i) {
        QVector3D a = polygon.points[i];
        QVector3D b = polygon.points[(i + 1) % polygon.points.size()];
        if (((a.y() > point.y()) != (b.y() > point.y())) &&
            (point.x() < (b.x() - a.x()) * (point.y() - a.y()) / (b.y() - a.y()) + a.x())) {
            count++;
        }
    }
    return (count % 2) == 1;
}

void PolygonExtrusionWidget::onApplyBooleanOperation() {
    if (polygons.size() < 2) {
        qDebug() << "At least two polygons are required.";
        return;
    }

    QString operation = booleanComboBox->currentText();
    Polygon &A = polygons[0];
    Polygon &B = polygons[1];

    QVector<QVector3D> resultPoints;

    if (operation == "Union") {
        // Perform union: Render a shape with no edges inside the intersection
        resultPoints = computeUnion(A, B);
        qDebug() << "Performed Union";
    } else if (operation == "Intersection") {
        // Perform intersection: Show only the intersected shape
        resultPoints = computeIntersection(A, B);
        qDebug() << "Performed Intersection";
    }
    else if (operation == "Subtraction")
    {
        resultPoints = computeSubtraction(A, B);
        qDebug() << "Performed Subtraction";
    }

    if (!resultPoints.isEmpty()) {
        A.points = resultPoints;
        A.isClosed = true;
        A.normal = computePolygonNormal(A.points);
        update();
    }
}

QVector<QVector3D> PolygonExtrusionWidget::computeIntersection(const Polygon &A, const Polygon &B) {
    QVector<QVector3D> result;

    // Convert QVector3D polygons to std::vector<std::vector<float>>
    std::vector<std::vector<float>> poly1, poly2;
    for (const auto &p : A.points) {
        poly1.push_back({p.x(), p.y()});
    }
    for (const auto &p : B.points) {
        poly2.push_back({p.x(), p.y()});
    }

    // Perform intersection using clipPolygon
    auto intersectionResult = clipPolygon(poly1, poly2);

    // Convert the result back to QVector3D
    for (const auto &p : intersectionResult) {
        result.append(QVector3D(p[0], p[1], 0.0f));
    }

    return result;
}

QVector<QVector3D> PolygonExtrusionWidget::computeUnion(const Polygon &A, const Polygon &B) {
    QVector<QVector3D> result;

    // Convert QVector3D polygons to std::vector<std::vector<float>>
    std::vector<std::vector<float>> poly1, poly2;
    for (const auto &p : A.points) {
        poly1.push_back({p.x(), p.y()});
    }
    for (const auto &p : B.points) {
        poly2.push_back({p.x(), p.y()});
    }

    // Perform union using convexHull
    std::vector<std::vector<float>> combined = poly1;
    combined.insert(combined.end(), poly2.begin(), poly2.end());
    auto unionResult = convexHull(combined);

    // Convert the result back to QVector3D
    for (const auto &p : unionResult) {
        result.append(QVector3D(p[0], p[1], 0.0f));
    }

    return result;
}

QVector<QVector3D> PolygonExtrusionWidget::computeSubtraction(const Polygon &A, const Polygon &B) {
    QVector<QVector3D> result;

    // Convert QVector3D polygons to std::vector<std::vector<float>>
    std::vector<std::vector<float>> poly1, poly2;
    for (const auto &p : A.points) {
        poly1.push_back({p.x(), p.y()});
    }
    for (const auto &p : B.points) {
        poly2.push_back({p.x(), p.y()});
    }

    // Perform subtraction using reverse Sutherland–Hodgman
    auto inside = clipPolygon(poly1, poly2);
    std::vector<std::vector<float>> outside;
    for (const auto &pt : poly1) {
        if (std::find(inside.begin(), inside.end(), pt) == inside.end()) {
            outside.push_back(pt);
        }
    }

    // Convert the result back to QVector3D
    for (const auto &p : outside.empty() ? inside : outside) {
        result.append(QVector3D(p[0], p[1], 0.0f));
    }

    return result;
}

bool PolygonExtrusionWidget::checkEdgeIntersection(const QVector3D &a1, const QVector3D &a2,
                                                   const QVector3D &b1, const QVector3D &b2,
                                                   QVector3D &intersection) {
    float x1 = a1.x(), y1 = a1.y();
    float x2 = a2.x(), y2 = a2.y();
    float x3 = b1.x(), y3 = b1.y();
    float x4 = b2.x(), y4 = b2.y();

    float denom = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
    if (qFuzzyIsNull(denom)) return false;

    float xi = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / denom;
    float yi = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / denom;

    if ((qMin(x1, x2) <= xi && xi <= qMax(x1, x2)) &&
        (qMin(y1, y2) <= yi && yi <= qMax(y1, y2)) &&
        (qMin(x3, x4) <= xi && xi <= qMax(x3, x4)) &&
        (qMin(y3, y4) <= yi && yi <= qMax(y3, y4))) {
        intersection = QVector3D(xi, yi, 0);
        return true;
    }
    return false;
}

static bool pointInPolygon(const std::vector<std::vector<float>> &polygon, float x, float y) {
    bool inside = false;
    size_t n = polygon.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const auto &pi = polygon[i];
        const auto &pj = polygon[j];
        if ((pi[1] > y) != (pj[1] > y) &&
            x < (pj[0] - pi[0]) * (y - pi[1]) / (pj[1] - pi[1] + 1e-6) + pi[0]) {
            inside = !inside;
        }
    }
    return inside;
}

std::vector<std::vector<float>> PolygonExtrusionWidget::clipPolygon(
    const std::vector<std::vector<float>> &subjectPolygon,
    const std::vector<std::vector<float>> &clipPolygon) {
    std::vector<std::vector<float>> output = subjectPolygon;
    for (size_t i = 0; i < clipPolygon.size(); ++i) {
        std::vector<std::vector<float>> input = output;
        output.clear();

        std::vector<float> A = clipPolygon[i];
        std::vector<float> B = clipPolygon[(i + 1) % clipPolygon.size()];

        auto inside = [&](const std::vector<float> &p) {
            return (B[0] - A[0]) * (p[1] - A[1]) > (B[1] - A[1]) * (p[0] - A[0]);
        };

        auto intersection = [&](const std::vector<float> &p1, const std::vector<float> &p2) {
            float A1 = p2[1] - p1[1];
            float B1 = p1[0] - p2[0];
            float C1 = A1 * p1[0] + B1 * p1[1];

            float A2 = B[1] - A[1];
            float B2 = A[0] - B[0];
            float C2 = A2 * A[0] + B2 * A[1];

            float det = A1 * B2 - A2 * B1;
            if (fabs(det) < 1e-6)
                return p2;
            float x = (B2 * C1 - B1 * C2) / det;
            float y = (A1 * C2 - A2 * C1) / det;
            return std::vector<float>{x, y};
        };

        for (size_t j = 0; j < input.size(); ++j) {
            const std::vector<float> &current = input[j];
            const std::vector<float> &prev = input[(j + input.size() - 1) % input.size()];
            bool currIn = inside(current);
            bool prevIn = inside(prev);

            if (currIn && prevIn)
                output.push_back(current);
            else if (prevIn && !currIn)
                output.push_back(intersection(prev, current));
            else if (!prevIn && currIn) {
                output.push_back(intersection(prev, current));
                output.push_back(current);
            }
        }
    }

    return output;
}

static float cross(const std::vector<float> &O, const std::vector<float> &A, const std::vector<float> &B) {
    return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0]);
}

std::vector<std::vector<float>> PolygonExtrusionWidget::convexHull(std::vector<std::vector<float>> P) {
    int n = P.size(), k = 0;
    if (n <= 3)
        return P;

    std::sort(P.begin(), P.end(), [](const std::vector<float> &a, const std::vector<float> &b) {
        return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
    });

    std::vector<std::vector<float>> H(2 * n);

    // Lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0)
            k--;
        H[k++] = P[i];
    }

    // Upper hull
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(H[k - 2], H[k - 1], P[i]) <= 0)
            k--;
        H[k++] = P[i];
    }

    H.resize(k - 1);
    return H;
}
