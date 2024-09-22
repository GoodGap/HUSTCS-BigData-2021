#2023/4/14
#大数据分析实验一
#Map-Reduce算法
#大数据2101班 李嘉鹏 U202115652

import sys
import threading #Python多线程库
import time

def read_input(file):
    for line in file:
        line = line.strip()
        yield line.split(', ')

def mapper(readfile, writefile):
    file = open(readfile)
    write = open(writefile, 'w')
    lines = read_input(file)
    with write as f:
        for words in lines:
            for word in words:
                f.write("{},{}\n".format(word, 1)) #map中，每个词出现一次即为一个键值对

if __name__ == '__main__':
    #创建map进程
    map_threads=[]
    for i in range(9):
        #循环9次，依次启动9个mapper进程
        t=threading.Thread(target=mapper(f"source0{i+1}",f"map{i+1}"))
        map_threads.append(t)
        t.start()
    start = time.perf_counter()
    #等待map进程结束
    count=1 #循环计数器
    for t in map_threads:
        t.join()
        print(f"map_t{count}: %s s" %(time.perf_counter() - start))  # 计算多线程运行时间
        count+=1