import sys
import threading
import time

def combiner(inputfile, outputfile):
    read = open(inputfile, 'r')
    write = open(outputfile, 'w')
    count_dict = {} #创建字典，存放每个词及其分别出现的次数
    for line in read:
        line = line.strip()
        word, count = line.split(',', 1)
        try:
            count = int(count)
        except ValueError:
            continue
        if word in count_dict.keys(): #已经查找到这个词，count+1
            count_dict[word] = count_dict[word] + count
        else: #第一次找到这个词，count=1
            count_dict[word] = count

    count_dict = sorted(count_dict.items(), key=lambda x: x[0], reverse=False)
    #对字典进行排序，依据单词key的字母序顺序排序
    for key, v in count_dict:
        write.write("{},{}\n".format(key, v))
    #输出格式：key,value
if __name__ == '__main__':
    combine_threads = []
    for i in range(9):
        t = threading.Thread(target=combiner(f"map{i + 1}", f"combine{i + 1}"))
        combine_threads.append(t)
        t.start()
    start = time.perf_counter()
    count = 1  # 循环计数器
    for t in combine_threads:
        t.join()
        print(f"combine_t{count}: %s s" % (time.perf_counter() - start))  # 计算多线程运行时间
        count += 1