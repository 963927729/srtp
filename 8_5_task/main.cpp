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

    angle begin;
    begin.alpha = 0.1; begin.beta = 0; begin.gama = 0;
    Matrix BEGIN(3,3); BEGIN = rotate_matrix( begin );

    ofstream out("data_end.txt");
    out << START.n << endl;
    for ( int i=0; i<START.n; i++) 
    {
        double a[3] = {START.d[i][0], START.d[i][1], START.d[i][2]};
        Matrix A(3,1); A.assign(a);
        Matrix B(3,1); B = B.mul_matrix(BEGIN,A);
        for( int j=0; j<3; j++ ) 
        {
            out << B.data[j][0];
            out << " ";
        }
        out << endl;
    
    }
    out.close();


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
    Matrix R(3,3);
    R = rotate_matrix( sita );

    double result = START.examine(R,END);
    cout << "the result of examine: " << endl;
    cout << result << endl;

   return 0;
}