#include "plane.h"
#include <math.h>
#include "matrix.h"
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

    Plane_Measure_Data START, END;
    Plane X(1,1,1,0);
    Plane Y(1,1,1,1);
    int T;

    // 读入初始点数据
    freopen("data_start.txt","r",stdin);
	START.read();
    START.out();
	X = START.LS(); // 进行最小二乘，得到平面X
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    // 读入结束点数据
    freopen("data_end.txt","r",stdin);
	END.read();
    END.out();
	Y = END.LS(); // 进行最小二乘，得到平面Y
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    // 计算偏转角
    node a,b;
    a.x = X.A; a.y = X.B; a.z = X.C;
    b.x = Y.A; b.y = Y.B; b.z = Y.C;
    angle sita = calc_angle(a,b);
    


    // 检验部分
    Matrix E(3,3), F(3,3), G(3,3),R(3,3);
    double e[9] = {1,0,0,
                   0,cos(sita.alpha),-sin(sita.alpha),
                   0,sin(sita.alpha),cos(sita.alpha)};
    E.assign(e);
    double f[9] = {cos(sita.beta),0,sin(sita.beta),
                   0,1,0,
                   -sin(sita.beta),0,cos(sita.beta)};
    F.assign(f);
    double g[9] = {
                   cos(sita.gama),-sin(sita.gama),0,
                   sin(sita.gama),cos(sita.gama),0,
                   0,0,1};
    G.assign(g);
    // 得到旋转矩阵
    R = R.mul_matrix(E,F); R = R.mul_matrix(R,G);
    R.Matrix_print();

    double result = START.examine(R,END);
    cout << "the result of examine: " << endl;
    cout << result << endl;

   return 0;
}