#ifndef MATH_H
#define MATH_H
#include <math.h>
#define PI 3.14159265358979323846
#include <cstdlib>  // For srand() and rand()
#include <QVector2D>


bool intersects(QVector2D A, QVector2D B, QVector2D C, QVector2D D, QVector2D &intersection);
bool intersectsRect(QVector2D A, QVector2D B, double x, double y, double width, double height);
bool contains(double x0, double y0, double x1, double y1, double test_x, double test_y);
double distance(QVector2D v0, QVector2D v1);
double distance(double x0, double y0, double x1, double y1);
double randn_notrig(double mu=0.0, double sigma=1.0);
double randn_trig(double mu=0.0, double sigma=1.0);
bool is_nan(double val);
double d_abs(double val);

#endif //MATH_H