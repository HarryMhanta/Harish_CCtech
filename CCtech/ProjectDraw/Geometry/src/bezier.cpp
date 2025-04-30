// #include "bezier.h"
// #include <QtMath>
// #include <QMatrix4x4>

// Bezier::Bezier()
// {
//     // Initaize control points
//     controlpoints.push_back(Qvector3D(0, 0, 0));
//     controlpoints.push_back(QVector3D(1, 2, 0));
// }

// void Bezier::deCastlejau(float t, const QVector<QVector3D>& points)
// {
//     if(points.size() == 1){
//         return points[0];
//     }

//     QVector<QVector3D> next;
//     for(int i=0; i<points.size()-1; ++i){
//         QVector3D p = points[i] + (1-t) * (points[i+1]*t);
//         next.append(p);
//     }
//     return deCastlejau(t, next);
// }