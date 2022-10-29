#include <arrayfire.h>
#include <iostream>

using namespace af;

array point2matrix(double& x, double& y, double& z, array& args)
{
    /*
    定义函数返回系数矩阵 B， l
        定义函数： point2matrix，
        通过给定的同名点坐标列立误差方程B系数阵的部分
        x, y, z： 原坐标值
        args: 七参数误差值[Delta_X, Delta_Y, Delta_Z, theta_x, theta_y, theta_z, m]
        返回值： W系数阵的部分
    */
    array W = constant(0,3,7,f64);
    W(seq(3), seq(3))= identity(3, 3);
    W(0, 4) = -(1 + args(6)) * z;
    W(0, 5) = (1 + args(6)) * y;
    W(0, 6) = x + args(5) * y - args(4) * z;

    W(1, 3) = (1 + args(6)) * z;
    W(1, 5) = -(1 + args(6)) * x;
    W(1, 6) = -(args(5) * x) + y + args(3) * z;

    W(2, 3) = -(1 + args(6)) * y;
    W(2, 4) = (1 + args(6)) * x;
    W(2, 6) = args(4) * x - args(3) * y + z;
    return W;
}


array points2W(array &points, array &args)
{
    /*
    定义函数： points2W
        通过同名点序列列立误差方程B系数阵的整体
        x, y, z： 同名点序列
        args: 七参数误差值[Delta_X, Delta_Y, Delta_Z, theta_x, theta_y, theta_z, m]
        返回值： W系数阵
    */
    array big_mat;
    double x, y, z;
    for(int i =0;i<points.dims(0);++i)
    {
        array p = points(i, span);
        x = p(0).scalar<double>();
        y = p(1).scalar<double>();
        z = p(2).scalar<double>();
        array mat = point2matrix(x, y, z, args);
        if (big_mat.elements() == 0)
        {
            big_mat = mat;
        }
        else
        {
            big_mat = join(0,big_mat, mat);
        }
    }
    return big_mat;
}

array ordinationConvert(double& x1, double& y1, double& z1, array& args)
{
    array p = constant(0,1,3,f64);
    p(0) = args(0) + (1 + args(6)) * (x1 + args(5) * y1 - args(4) * z1);
    p(1) = args(1) + (1 + args(6)) * (-(args(5) * x1) + y1 + args(3) * z1);
    p(2) = args(2) + (1 + args(6)) * (args(4) * x1 - (args(3) * y1) + z1);
    return p;
}

array points2b(array &source_points, array &target_points, array &args)
{
    /*
    定义函数： points2b
          通过同名点坐标转换关系列立误差方程B系数阵的整体
          x, y, z： 同名点的原坐标和目标坐标对组成的序列
          args: 七参数误差值[Delta_X, Delta_Y, Delta_Z, theta_x, theta_y, theta_z, m]
          返回值： b系数阵
    */

    int len = source_points.dims(0);
    array big_mat = constant(0,len,3,f64);
    //array p0, p1, p2;
    double x1, y1, z1;
    for(int i=0;i<len;++i)
    {
        array p1 = source_points(i, span);
        x1 = p1(0).scalar<double>(); y1 = p1(1).scalar<double>(); z1 = p1(2).scalar<double>();
        array p2 = target_points(i, span);
        array p0 = ordinationConvert(x1, y1, z1, args);
        big_mat(i, span) = p2 - p0;
    }
    return flat(big_mat.T());
}

int main()
{
    try {
        /*int device = 2;
        setDevice(device);*/
        array Args = constant(0, 7,f64);
        array parameters = constant(1,7,f64);
        // 原坐标(x, y, z)
        double h_source_points[12] = {
            3381400.980, 395422.030, 32.956,
            3381404.344, 395844.239, 32.207,
            3382149.810, 396003.592, 33.290,
            3382537.793, 395985.359, 33.412 };
        // 目标坐标（x’, y’, z’)  
        double h_target_points[12] = {
            3380968.194, 539468.888, 13.875,
            3380977.154, 539890.934, 13.179,
            3381724.612, 540040.47, 14.273,
            3381724.636, 540040.485, 14.282 };

        array source_points = array(3, 4, h_source_points).T();
        array target_points = array(3, 4, h_target_points).T();

        //归一化处理便于模型的快速迭代
        array ave_src = mean(source_points,0);
        array ave_tar = mean(target_points,0);
        source_points -= tile(ave_src,4,1,1,1);
        target_points -= tile(ave_tar,4,1,1,1);

        timer::start();
        // 当七参数的误差值之和大于1e - 10时，迭代运算得到更精确的结果
        array W, b, qxx;
        while ((sum)(abs(parameters)).scalar<double>() > 1e-10)
        {
            W = points2W(source_points, Args);
            b = points2b(source_points, target_points, Args);
            qxx = inverse(matmul(W.T(), W));
            parameters = matmul(matmul(qxx, W.T()), b);
            Args += parameters;
        }

        //af_print(parameters); 
        std::cout << "七参数输出: " << std::endl;
        af_print(Args);

        // 检查点坐标
        double hsource_test_points[3] = { 3381402.058, 395657.940, 32.728 };
        double htarget_test_points[3] = { 3380972.424, 539704.811, 13.665 };

        array source_test_points(1, 3, hsource_test_points);
        array target_test_points(1, 3, htarget_test_points);

        // 归一化处理
        source_test_points = source_test_points - ave_src;

        // 单位权标准差，即所得模型的坐标精度
        double sigma0 = (sqrt)(sum(b * b) / 2).scalar<double>();
        // 参数标准差，即所得模型的七参数精度
        array sigmax = sigma0 * (sqrt)(diag(qxx));
 
        double x, y, z;
        x = source_test_points(0).scalar<double>(); 
        y = source_test_points(1).scalar<double>(); 
        z = source_test_points(2).scalar<double>();
        array p_test = ordinationConvert(x, y, z, Args) + ave_tar;

        std::cout << "单位权中误差: " << sigma0 << std::endl;

        std::cout << "参数中误差:" << std::endl;
        af_print(sigmax);
        std::cout << "模型预测结果: " << std::endl;
        af_print(p_test);
        std::cout << "真实结果: " << std::endl;
        af_print(target_test_points);
        std::cout << "消耗时间: " << timer::stop() <<" s" << std::endl;
    }
    catch (af::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
