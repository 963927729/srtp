import numpy as np

source_points = [
    (3381400.980, 395422.030, 32.956),
    (3381404.344, 395844.239, 32.207),
    (3382149.810, 396003.592, 33.290),
    (3382537.793, 395985.359, 33.412)
]
target_points = [
    (3380968.194, 539468.888, 13.875),
    (3380977.154, 539890.934, 13.179),
    (3381724.612, 540040.47,  14.273),
    (3381724.636, 540040.485, 14.282)
]

#归一化处理便于模型的快速迭代
ave_src = np.mean(np.array(source_points), axis=0)
ave_tar = np.mean(np.array(target_points), axis=0)
source_points -= ave_src
target_points -= ave_tar

#  定义函数返回系数矩阵 B， l
#  定义函数： point2matrix，
#     通过给定的同名点坐标列立误差方程B系数阵的部分
#     x, y, z： 原坐标值
#    args: 七参数误差值[Delta_X, Delta_Y, Delta_Z, theta_x, theta_y, theta_z, m]
#   返回值： W系数阵的部分
       
def point2matrix(x, y, z, args):
    array = [
        [1, 0, 0,              0, -(1+args[6])*z,  (1+args[6])*y,          x + args[5]*y - args[4]*z],
        [0, 1, 0,  (1+args[6])*z,              0, -(1+args[6])*x, -args[5]*x +         y + args[3]*z],
        [0, 0, 1, -(1+args[6])*y,  (1+args[6])*x,              0,  args[4]*x - args[3]*y +         z]
    ]
    return np.array(array)

# 定义函数： points2W
#      通过同名点序列列立误差方程B系数阵的整体
#       x, y, z： 同名点序列
#       args: 七参数误差值[Delta_X, Delta_Y, Delta_Z, theta_x, theta_y, theta_z, m]
#       返回值： W系数阵

def points2W(points, args):   
    big_mat = None 
    for (x, y, z) in points:
        mat = point2matrix(x, y, z, args)
        if big_mat is None:
            big_mat = mat
        else:
            big_mat = np.concatenate((big_mat, mat))

    return big_mat

# 定义函数： points2b
#       通过同名点坐标转换关系列立误差方程B系数阵的整体
#       x, y, z： 同名点的原坐标和目标坐标对组成的序列
#       args: 七参数误差值[Delta_X, Delta_Y, Delta_Z, theta_x, theta_y, theta_z, m]
#       返回值： b系数阵

def points2b(source, target, args):   
    big_mat = [0] * len(source) * 3

    for i, ((x1, y1, z1), (x2, y2, z2)) in enumerate(zip(source, target)):
        (x0, y0, z0) = ordinationConvert(x1, y1, z1, args)
        big_mat[3*i + 0] = x2 - x0
        big_mat[3*i + 1] = y2 - y0
        big_mat[3*i + 2] = z2 - z0

    return np.array(big_mat).transpose()

def ordinationConvert(x1, y1, z1, args):
    x2 = args[0] + (1+args[6])*(         x1 + args[5]*y1 - args[4]*z1)
    y2 = args[1] + (1+args[6])*(-args[5]*x1 +         y1 + args[3]*z1)
    z2 = args[2] + (1+args[6])*( args[4]*x1 - args[3]*y1 +         z1)
    return (x2, y2, z2)


Args = np.array([0, 0, 0, 0, 0, 0, 0], dtype='float64')
parameters = np.array([1, 1, 1, 1, 1, 1, 1])

# 当七参数的误差值之和大于1e-10时，迭代运算得到更精确的结果
while np.fabs(np.array(parameters)).sum() > 1e-10 :
    W = points2W(source_points, Args)
    b = points2b(source_points, target_points, Args)
    qxx = np.linalg.inv(np.dot(W.transpose(), W))
    parameters = np.dot(np.dot(qxx, W.transpose()), b)
    Args += parameters
	
# 打印七参数
print("Args:")
print(np.round(Args, 3))


# 检查点坐标
source_test_points = [
    (3381402.058, 395657.940, 32.728)
]

target_test_points = [
    (3380972.424, 539704.811, 13.665)
]

#归一化处理
source_test_points = np.array(source_test_points - ave_src)

# 单位权标准差，即所得模型的坐标精度
sigma0 = np.sqrt((b*b).sum() / 2)
# 参数标准差，即所得模型的七参数精度
sigmax = sigma0 * np.sqrt(np.diag(qxx))
print('单位权中误差: %.3f' % (sigma0))
print('参数中误差:')
print(np.round(sigmax,3))
(x2, y2, z2) = ordinationConvert(source_test_points[0, 0], source_test_points[0, 1], source_test_points[0, 2], Args) + ave_tar
print('模型预测结果: ')
print('[(%.3f, %.3f, %.3f)]'%(x2, y2, z2))
print('真实结果: ')
print(target_test_points)