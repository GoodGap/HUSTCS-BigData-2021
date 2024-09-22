#2023/4/21
#大数据分析实验四
#聚类算法（K-Means）
#大数据2101班 李嘉鹏 U202115652

import pandas as pd
import numpy as np
import random as rd
import matplotlib.pyplot as plt
import csv

def len_s(center, data):
    s = 0
    for i in range(1, 14):
        s += pow(center[i]-float(data[i]), 2)
    return s

def ssenum(m, data_min):   # 分别计算三种聚类的距离平方和SSE
    sse_num = [0, 0, 0]
    for i in range(m):
        sse_num[int(data_min[i, 0])-1] += data_min[i, 1]
    print('三个聚类的SSE：', sse_num)
    return sse_num

def get_points(m, data, j, data_min): # 根据求得的每个点的信息分类
    point = []
    for i in range(m):
        if data_min[i, 0] == j + 1:  # 根据保存的信息进行分类
            point.append(data[i])
    return point

def get_acc(m, data, k, data_min):
    hit = 0
    for j in range(k):
        cluster_tmp = [0, 0, 0]
        point = get_points(m, data, j, data_min)
        for item in point:
            cluster_tmp[int(item[0])-1] += 1
        hit += max(cluster_tmp)
    acc = hit / len(data)
    print('准确度(acc)：', acc)    # 准确度为正确分类数与总数之比

if __name__ == '__main__':
    with open('归一化数据.csv') as f:
        reader = csv.reader(f)
        data = []
        for j in reader:
            j = list(map(float, j))
            data.append(j)
    m = len(data)
    k = 3
    change = True  # 标记两次循环前后有无变化
    data_min = np.mat(np.zeros((m, 2)))
    centers = []
    for i in range(k):
        c = []
        c.append(0)
        for j in range(13):
            c.append(rd.random())  # 随机生成三个13维的初始中心点
        centers.append(c)
    count = 0
    while change:
        count += 1
        change = False
        for i in range(m):
            min_s = 999999.0
            min_center = -1
            for j in range(k):
                distance = len_s(centers[j], data[i])
                if distance < min_s:
                    min_center = j + 1
                    min_s = distance             # 计算每个点到中心点的距离并得到最小值
            if data_min[i, 0] != min_center or data_min[i, 1] != min_s: #中心点变化，或者是到中心点的距离发生变化
                data_min[i, :] = min_center, min_s
                change = True       # 更新每个点的最近中心点和距离信息
        for j in range(k):
            point = get_points(m, data, j, data_min)  # 根据求得的每个点的信息分类
            centers = np.array(centers)
            centers[j, :] = np.mean(point, axis=0)    # 计算出新的中心点,即列的均值
    print('循环次数：', count)
    ssenum(m, data_min)
    get_acc(m, data, k, data_min)
    colors = ['r', 'g', 'b', 'y', 'c', 'm']
    print('\n请输入x轴与y轴对应的属性：')
    print('x')
    x = int(input())
    print('y')
    y = int(input())
    for j in range(k):
        point = get_points(m, data, j, data_min)
        point = np.array(point)
        plt.scatter(point[:, x], point[:, y], c=colors[j])
    plt.scatter(centers[:, x], centers[:, y], marker='*', s=150, c='black')
    plt.show()
    with open("result.csv", 'w') as f:
        for i in range(m):
            f.write("{}\t,{}\n".format(data_min[i, 0], data_min[i, 1]))