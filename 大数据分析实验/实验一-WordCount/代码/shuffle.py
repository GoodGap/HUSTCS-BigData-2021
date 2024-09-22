import sys
import threading
import time

def shuffle(readfile):
    file = open(readfile)
    write1 = open('shuffle1', 'a') #以追加方式打开文件，三个文件存的单词由hash函数决定
    write2 = open('shuffle2', 'a')
    write3 = open('shuffle3', 'a')
    for line in file:
        line = line.strip()
        word, count = line.split(',', 1)
        if (word[0] >= 'a' and word[0]<='z' and hash(word)%3==0) or (word[0] >= 'A' and word[0]<='Z' and hash(word)%3==0): #利用哈希函数对word进行shuffle
            write1.write("{},{}\n".format(word, count))
        elif (word[0] >= 'a' and word[0]<='z' and hash(word)%3==1) or (word[0] >= 'A' and word[0]<='Z' and hash(word)%3==0):
            write2.write("{},{}\n".format(word, count))
        else:
            write3.write("{},{}\n".format(word, count))
        #没有直接使用特定首字母mod3的方式（如下），避免了负载不均衡性
        # if (word[0] >= 'a' and word[0]<='z' and ord(word[0])%3==0) or (word[0] >= 'A' and word[0]<='Z' and ord(word[0])%3==0)
        # elif (word[0] >= 'a' and word[0]<='z' and ord(word[0])%3==1) or (word[0] >= 'A' and word[0]<='Z' and ord(word[0])%3==1)

if __name__ == '__main__':
    shuffle_threads = []
    for i in range(9):
        # 循环9次，依次启动9个mapper进程
        t = threading.Thread(target=shuffle(f"combine{i + 1}"))
        shuffle_threads.append(t)
        t.start()
    start = time.perf_counter()
    # 等待map进程结束
    count = 1  # 循环计数器
    for t in shuffle_threads:
        t.join()
        print(f"shuffle_t{count}: %s s" % (time.perf_counter() - start))  # 计算多线程运行时间
        count += 1