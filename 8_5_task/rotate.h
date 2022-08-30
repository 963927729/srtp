#ifndef ROTATE_H
#define ROTATE_H

#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include "matrix.h"

#define pi 3.14159
using namespace std;

// 定义点 node
struct node
{
	double x,y,z;
	void out()
	{
		printf("%lf %lf %lf\n",x,y,z);
	}
	double len2()
	{
		return x*x+y*y+z*z;
	}
	double len()
	{
		return sqrt(x*x+y*y+z*z);
	}

	node operator /(double n)
	{
		(*this).x = (*this).x/n; (*this).y = (*this).y/n; (*this).z = (*this).z/n;
		return *this;
	}
	
};

node operator -(node a,node b){return (node){a.x-b.x,a.y-b.y,a.z-b.z};}
node operator +(node a,node b){return (node){a.x+b.x,a.y+b.y,a.z+b.z};}


// 定义偏转角度 angle
struct angle
{
    double alpha, beta, gama;
	// 定义外积
    node exterior( node a, node b)
    {
        node result;
        result.x = a.y*b.z - a.z*b.y;
        result.y = a.z*b.x - a.x*b.z;
        result.z = a.x*b.y - a.y*b.x;
        return result;
    }
	// 定义内积
    double inner( node a, node b)
    {
        return (a.x*b.x + a.y*b.y + a.z*b.z);
    }

	void out()
	{
		cout << "The Deflection Angle is :" << endl;
		cout << " alpha = " << alpha << " ; beta = " << beta << " ; gama = " << gama << " ;" << endl;
	}
};


angle calc_angle ( node a, node b)
{
    a = a/a.len();  b = b/b.len();
	// 计算 v,s,c
	angle x_1, x_2;
    node v = x_1.exterior(a,b);
	double s = v.len();
    double c = x_1.inner(a,b);
	cout << "c = " << c << " s = " << s << endl;
	v.out();

	// 计算所需矩阵
	double d[9] = {0, -v.z, v.y, v.z, 0, -v.x, -v.y, v.x, 0};
	Matrix D(3,3);  D.assign(d);
	double e[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1}; 
	Matrix E(3,3);  E.assign(e);
	Matrix F = F.mul_matrix(D,D);
	F = F * (1/(1+c));
	D.Matrix_print();

	// 得到最终矩阵 R
	Matrix H = H.add_matrix(E, D);
	Matrix R = R.add_matrix(H, F);
	R.Matrix_print();

	//计算偏转角 alpha, beta, gama
	if( R.data[2][0] != 1 && R.data[2][0] != -1 )
	{
		x_1.alpha = -asin(R.data[2][0]);
		x_2.alpha = pi - x_1.alpha;
		x_1.beta = atan2( R.data[2][1]/cos(x_1.alpha), R.data[2][2]/cos(x_1.alpha));
		x_2.beta = atan2( R.data[2][1]/cos(x_2.alpha), R.data[2][2]/cos(x_2.alpha));
		x_1.gama = atan2( R.data[1][0]/cos(x_1.alpha), R.data[0][0]/cos(x_1.alpha));
		x_2.gama = atan2( R.data[1][0]/cos(x_2.alpha), R.data[0][0]/cos(x_2.alpha));
		x_1.out(); x_2.out();
		return x_1;
	} else
	{
		x_1.gama = 0;
		if( R.data[2][0] == -1 )
		{
			x_1.alpha = pi/2;
			x_1.beta = x_1.gama + atan2( R.data[0][1], R.data[0][2]);
		} else
		{
			x_1.alpha = -pi / 2;
			x_1.beta = -x_1.gama + atan2( -R.data[0][1], -R.data[0][2]);
		}
		return x_1;
	}

}

// 旋转矩阵
Matrix rotate_matrix( angle sita )
{
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
    return R;
}

#endif

