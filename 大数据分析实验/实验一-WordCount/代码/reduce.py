import sys
import threading
import time

def reduce(readfile, writefile):
    file = open(readfile, 'r')
    write = open(writefile, 'w')
    count_dict = {}
    for line in file:
        line = line.strip()
        word, count = line.split(',', 1)
        try:
            count = int(count)
        except ValueError:
            continue
        if word in count_dict.keys():
            count_dict[word] = count_dict[word] + count
        else:
            count_dict[word] = count

    count_dict = sorted(count_dict.items(), key=lambda x: x[0], reverse=False)
    for key, v in count_dict:
        write.write("{},{}\n".format(key, v))

if __name__ == '__main__':
    t1 = threading.Thread(target=reduce('shuffle1', 'reduce1'), args=("t1",))
    t2 = threading.Thread(target=reduce('shuffle2', 'reduce2'), args=("t2",))
    t3 = threading.Thread(target=reduce('shuffle3', 'reduce3'), args=("t3",))
    start = time.perf_counter()
    t1.start()
    t2.start()
    t3.start()

    t1.join()
    print("reduce_1 %s s" % (time.perf_counter() - start))
    t2.join()
    print("reduce_2 %s s" % (time.perf_counter() - start))
    t3.join()
    print("reduce_3 %s s" % (time.perf_counter() - start))