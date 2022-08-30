#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <math.h>
#include <bits/stdc++.h>
#include "matrix.h"

using namespace std;
const int N=20;
const double eps=1e-8;

struct Plane
{
    double A,B,C,D;
    // 初始化
    Plane( double a, double b, double c, double d)
    {
        (*this).A = a;
        (*this).B = b;
        (*this).C = c;
        (*this).D = d;
    }
    // 输出平面方程
    void out()
    {
        cout << "The plane equation is : " << endl;
        cout << A << "x + " << B << "y + " << C << "z + " << D << " = 0 " << endl;
    }
    // 求 A,B,C 的模
    double len2()
    {
        return sqrt( A*A + B*B + C*C );
    }
    // 定义运算符 /
    Plane operator /(double n)
    {
        (*this).A = (*this).A / n;
        (*this).B = (*this).B / n;
        (*this).C = (*this).C / n;
        (*this).D = (*this).D / n;
    }
};

struct Plane_Measure_Data 
{
	int n;
	double d[N][3];
    // 读入n个点的坐标值
	void read()
	{
		scanf("%d",&n);
		for(int i=0;i<n;i++)
			for(int j=0;j<3;j++)scanf("%lf",&d[i][j]);
		
	}
    // 输出n个点的坐标值
	void out()
	{
		printf("%d\n",n);
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<3;j++)printf("%lf ",d[i][j]);
			puts("");
		}
	}
    // 便于后边的计算
    double LS_helper(int a, int b) // 其中 0 表示 1, 1 表示 x ,2 表示 y, 3 表示 z
    {
        double sum = 0;
        for(int i=0; i<n; i++)
        {
            if (b==0) { sum += d[i][a]; }
            else { sum += d[i][a] * d[i][b]; };
            
        }

        return sum;
    }
    // 对n个点进行最小二乘得到平面
    Plane LS()
    {   
        double m = 1.0*n;
        double d[9] = {LS_helper(1,1),LS_helper(1,2),LS_helper(1,0),
                       LS_helper(1,2),LS_helper(2,2),LS_helper(2,0),
                       LS_helper(1,0),LS_helper(2,0),m};
        double e[3] = {LS_helper(1,3),LS_helper(2,3),LS_helper(3,0)};
        Matrix D(3,3); D.assign(d);
        Matrix E(3,1); E.assign(e);

        Matrix F = F.inv_matrix(D); // 求逆
        Matrix G = G.mul_matrix(F,E);// 最终的列向量[a_0,a_1,a_2]
        G.Matrix_print();
        Plane result(G.data[0][0], G.data[1][0], -1, G.data[2][0]);
        result.out();

        return result;
    }
    // 检验算法是否正确
    double examine(Matrix A, Plane_Measure_Data B)
    {
        double result = 0;
        for( int i=0; i<n; i++ )
        {
            double a[3] = {(*this).d[i][0], (*this).d[i][1], (*this).d[i][2]};
            Matrix x(3,1); x.assign(a);
            Matrix y(3,1); y = y.mul_matrix(A,x);
            double sum = 0;
            for( int j=0; j<3; j++ )
            {
                sum += (B.d[i][j] - y.data[j][0]) * (B.d[i][j] - y.data[j][0]);
            }
            sum = sqrt(sum);
            cout << i << "~~~~~~~" << sum << endl;
            result += sum;
        }
        return result;
    }
};


#endif

