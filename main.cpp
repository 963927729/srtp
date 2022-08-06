#include "plane.h"
#include "rotate.h"
#include "iostream"
#include <bits/stdc++.h>

int main()
{
    /*
    Matrix A(3,3);
    A.create();
    A.Matrix_print();
    cout << A.col << " and " << A.row << endl;
    cout << A.data[2][2] << endl;
    Matrix C = C.add_matrix(A,A);
    C.Matrix_print();
    Matrix D = D.mul_matrix(A,C);
    D.Matrix_print();
    */

    Plane_Measure_Data S;
    Plane X(1,1,1,0);
    Plane Y(1,1,1,1);
    int T;

    // 读入初始点数据
    freopen("data_start.txt","r",stdin);
	S.read();
    S.out();
	X = S.LS(); // 进行最小二乘，得到平面X
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    // 读入结束点数据
    freopen("data_end.txt","r",stdin);
	S.read();
    S.out();
	Y = S.LS(); // 进行最小二乘，得到平面Y
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    // 计算偏转角
    node a,b;
    a.x = X.A; a.y = X.B; a.z = X.C;
    b.x = Y.A; b.y = Y.B; b.z = Y.C;
    angle sita = calc_angle(a,b);
    

   return 0;
}