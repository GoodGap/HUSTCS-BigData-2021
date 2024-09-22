#2023/4/14
#大数据分析实验二
#Pagerank实现（包含teleport）
#大数据2101班 李嘉鹏 U202115652

import numpy as np

if __name__ == '__main__':
    f = open('sent_receive.csv')
    edges = [line.strip('\n').split(',') for line in f]
    nodes = []
    for edge in edges:
        if edge[1] not in nodes:
            nodes.append(edge[1])
        if edge[2] not in nodes:
            nodes.append(edge[2])#遍历发件人和收件人，找到不重复的结点

    N = len(nodes)
    M = np.zeros([N, N]) #构建N*N的邻接矩阵M
    for edge in edges:
        start = nodes.index(edge[1])
        end = nodes.index(edge[2])
        M[end, start] = 1  # 初始化M，使得(sent,receive)位置上的值为1，其它位置的值为0

    for j in range(N):
        sum_of_col = sum(M[:, j])#取得每一列的矩阵之和
        for i in range(N):
            if M[i, j]:
                M[i, j] /= sum_of_col  # 标准化，使M每一列的和为1

    r = np.ones(N) / N
    next_r = np.zeros(N)
    e = 1  # 误差初始化
    k = 0  # 记录迭代次数
    b = 0.9 #进阶版pagerank公式，其中β=0.85（可改成0.8/0.9等）

    while e > 0.00000001:  # 迭代到误差小于10e-8停止
        next_r = np.dot(M, r) * b + (1-b) / N * np.ones(N)   # 迭代公式
        sum_of_col = sum(next_r)
        next_r = next_r / sum_of_col #确保整个矩阵值的和保持为1
        e = next_r - r
        e = max(map(abs, e))  # 计算误差
        r = next_r
        k += 1

    for i in range(N):
        print('人名id：%s' % r[i])
        print('Pagerank值：%s' % nodes[i])
    print('迭代了%s次' % str(k))
    print("r矩阵中元素之和：%f" % sum(r))