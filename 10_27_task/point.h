#ifndef _POINT_H
#define _POINT_H

#include <iostream>

using namespace std;
#define N 10

class Point { //类的定义
public: //外部接口
    Point();
    Point(double x, double y, double z);
    ~Point();
    void move(double newX, double newY, double newZ);
    void out();
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    static void showCount(); //静态函数成员
private: //私有数据成员
    double x, y, z;
};


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
void Point::out()
{
    cout << "The point is (" << x << ", " << y << ", " << z <<")" << endl;
}
#endif //_POINT_H



