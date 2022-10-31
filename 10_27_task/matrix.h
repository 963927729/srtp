#ifndef MATRIX_H
#define MATRIX_H
#include <bits/stdc++.h>
#include <iostream>
#include <math.h>


using namespace std;
#define Max 10

// 定义矩阵算法
struct Matrix
{
	int row, col; // 行，列
	double **data;
	int i,j;

	Matrix(int a, int b)
	{
		row = a;
		col = b;
		data = new double *[row];
		for(  i=0; i<row; i++ )
			data[i] = new double [col];
	}
	// 初始化矩阵
	void create()
	{	
		cout << "Please enter Matrix :" << endl;
		for( i=0; i<row; i++ )
		   for( j=0; j<col; j++ )
				cin >> data[i][j];

	}

	// 输出矩阵
	void Matrix_print()
	{
		cout << "print the matrix" << endl;
		for( i=0; i<row; i++ )
		{
			for( j=0; j<col; j++ )
			{
				if( j== col-1 )
				{
					cout << data[i][j] << endl;
				} else 
				{
					cout << data[i][j] << "  ";
				}
			}
		}
	}
    // 将一维数组的值复制到矩阵中
	Matrix assign(double *x) //复制运算符=重载，只能重载为成员函数

	{  
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				(*this).data[i][j] = x[i*col+j];
		return *this;
	}
    // 将二维数组的值复制到矩阵中
    Matrix copy_Matrix(double x[Max][Max])
    {
        for(int i=0; i<row; i++)
        {
            for(int i=0; i<col; i++)
            {
                (*this).data[i][j] = x[i][j];
            }
        }
        return *this;
    }

    Matrix concatenate( Matrix a, Matrix b)
    {
        int row = a.row + b.row;
        Matrix c(row,a.col);
        for( int i=0; i<row; i++ )
        {
            for( int j=0; j<col; j++ )
            {
                if( i < a.row ) {c.data[i][j] = a.data[i][j];}
                else {c.data[i][j] = b.data[i-a.row][j];}
            }
        }
        return c;
    }

    // 矩阵转置
    Matrix transpose( Matrix A)
    {
        Matrix B(A.col, A.row);
        for( int i=0; i<B.row; i++ )
        {
            for( int j=0; j<B.col; j++ )
            {
                B.data[i][j] = A.data[j][i];
            }
        }
        return B;
    }

	Matrix operator *( double n)
	{
		for( int i=0; i<row; i++ )
			for( int j=0; j<col; j++ )
				(*this).data[i][j] *= n;
		return *this;
	}
	// 矩阵加法
	Matrix add_matrix( Matrix A, Matrix B )
	{
		Matrix C(A.row, A.col);
		for( int i=0; i<C.row; i++ )
		{
			for( int j=0; j<C.col; j++ )
			{
				C.data[i][j] = A.data[i][j] + B.data[i][j];
			}
		}
		return C;
	}

	// 矩阵乘法
	Matrix mul_matrix( Matrix A, Matrix B )
	{
		Matrix C(A.row, B.col);
		for( int i=0; i<C.row; i++ )
		{
			for( int j=0; j<C.col; j++ )
			{
				double sum = 0.0;
				for( int k=0; k<A.col; k++ )
				{
					sum += A.data[i][k] * B.data[k][j];
				}
				C.data[i][j] = sum;
			}
		}
		return C;
	}
    // 定义除法运算符 /
    Matrix operator /(double n)
    {
        for( int i=0; i<row; i++)
        {
            for( int j=0; j<col; j++ )
            {
                (*this).data[i][j] /= n;
            }
        }
        return *this;
    }
    // 矩阵求解行列式
    double det_matrix( Matrix A )
    {
        if( A.col != A.row )
        {
            cout << "The wrong input!" << endl;
            return 0;
        } else 
        {
            int n = A.col;
            if( n==1 )
            {
                return A.data[0][0];
            }
            double ans = 0;
            double p[(n-1)*(n-1)];
            double temp[n-1][n-1]={0.0};
            for( int i=0; i<n; i++ )
            {
                for( int j=0; j<n-1; j++ )
                {
                    for( int k=0; k<n-1; k++ )
                    {
                        temp[j][k] = A.data[j+1][(k>=i)?k+1:k];
                        p[j*(n-1)+k] = temp[j][k];
                    }
                }

                Matrix B(n-1, n-1); B.assign(p);
                double t = det_matrix(B);
                if(i%2 == 0)
                {
                    ans += A.data[0][i]*t;
                }
                else
                {
                    ans -= A.data[0][i]*t;
                }
            }
            return ans;
        }
        
    }
    // 矩阵求逆
    Matrix inv_matrix( Matrix A )
    {
        Matrix B(A.row, A.col);
        if( A.row != A.col )
        {
            cout << "The wrong input!" << endl;
            return A;
        } else
        {
            if( A.col == 1 )
            {
                B.data[0][0] = 1;
                return B;
            }
            int n = A.col;
            double p[(n-1)*(n-1)];
            double temp[n-1][n-1];
            for( int i=0; i<n; i++ )
            {
                for( int j=0; j<n; j++ )
                {
                    for( int k=0; k<n-1; k++ )
                    {
                        for( int t=0; t<n-1; t++ )
                        {
                            temp[k][t] = A.data[k>=i?k+1:k][t>=j?t+1:t];
                            p[k*(n-1)+t] = temp[k][t];
                        }
                    }
                    
                    
                    Matrix C(n-1, n-1); C.assign(p);
                    B.data[j][i] = C.det_matrix(C);
                    if((i+j)%2 == 1)
                    {
                        B.data[j][i] = -B.data[j][i];
                    }
                }
            }
            double det = A.det_matrix(A);

            if( det == 0 )
            {
                cout << "The determinant of the original matrix is 0!" <<endl;
                return A;
            } else
            {
                B = B / det;
                return B;
            }
        }

    }

};





#endif

