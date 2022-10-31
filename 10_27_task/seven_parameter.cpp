#include <cstdio>
#include <iostream>
#include <bits/stdc++.h>
#include "matrix.h"
#include "point.h"


// 返回 W 系数阵的部分
Matrix point2matrix(double x, double y, double z, double *args)
{
    double array[21] = {
        1, 0, 0,              0, -(1+args[6])*z,  (1+args[6])*y,          x + args[5]*y - args[4]*z,
        0, 1, 0,  (1+args[6])*z,              0, -(1+args[6])*x, -args[5]*x +         y + args[3]*z,
        0, 0, 1, -(1+args[6])*y,  (1+args[6])*x,              0,  args[4]*x - args[3]*y +         z
    };
    Matrix result(3,7); result.assign(array);
    return result;
}


// 求解系数矩阵 W
Matrix points2W(Point *points, int n, double *args)
{
    Matrix big_mat(3*n,7);
    for( int i=0; i<n; i++ )
    {
        Matrix mat(3,7); mat = point2matrix(points[1].getX(), points[1].getY(), points[1].getZ(), args);
        Matrix bigger_mat = big_mat.concatenate(big_mat, mat);

    }
    return big_mat;
}


Point ordinationConvert(double x1, double y1, double z1, double *args)
{
    double x2 = args[0] + (1+args[6])*(         x1 + args[5]*y1 - args[4]*z1);
    double y2 = args[1] + (1+args[6])*(-args[5]*x1 +         y1 + args[3]*z1);
    double z2 = args[2] + (1+args[6])*( args[4]*x1 - args[3]*y1 +         z1);
    Point result(x2,y2,z2);
    return result;
}


Matrix points2b(Point *source, Point *target, int n, double *args)
{
    Matrix big_mat(3,7);
    return big_mat;
}

double form(double *a, int n)
{
    double result=0;
    for(int i=0; i<n; i++)
    {
        result += abs(a[i]);
    }
    return result;
}

int main()
{
    // 旋转前的点
    freopen("data_start.txt","r",stdin);

    int n; double data[N][3];
	scanf("%d",&n);
	for(int i=0;i<n;i++)
		for(int j=0;j<3;j++)scanf("%lf",&data[i][j]);	

    Point source_points[n];
    for( int i=0; i<n; i++)
    {
        source_points[i].move(data[i][0], data[i][1], data[i][2]);
    }

    // 旋转后的点
    freopen("data_start.txt","r",stdin);
    int n_2;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
		for(int j=0;j<3;j++)scanf("%lf",&data[i][j]);	
    Point target_points[n];
    for( int i=0; i<n; i++)
    {
        target_points[i].move(data[i][0], data[i][1], data[i][2]);
    }

    double args[7] = {0,0,0,0,0,0,0};
    double parameters[7] = {1,1,1,1,1,1,1};

    while (form(parameters,7) > 1e-10) 
    {
        Matrix W = points2W(source_points, n, args);
        Matrix b = points2b(source_points, target_points, n, args);
        Matrix WW = W.mul_matrix(W.transpose(W), W);
        Matrix qxx = WW.inv_matrix(WW);
        Matrix T = qxx.mul_matrix(qxx, W.transpose(W));
        Matrix P = T.mul_matrix(T, b);
        for( int i=0; i<7; i++ )
        {
            parameters[i] = P.data[i][0];
            args[i] += parameters[i];
        }
    }   
    
    cout << "Args: " ;
    for( int i=0; i<7; i++) {cout << args[i] << " ";}
    cout << endl;

}

