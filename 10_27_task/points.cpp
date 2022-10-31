//Point.cpp
#include <iostream>
#include "point.h"

using namespace std;

Point::Point() : x(0), y(0), z(0) {
}
Point::Point(double x, double y, double z) : x(x), y(y), z(z) {
}
Point::~Point() {}
void Point::move(double newX, double newY,double newZ) {
cout << "Moving the point to (" << newX << ", " << newY << ", " << newZ <<")" << endl;
x = newX;
y = newY;
z = newZ;
}
