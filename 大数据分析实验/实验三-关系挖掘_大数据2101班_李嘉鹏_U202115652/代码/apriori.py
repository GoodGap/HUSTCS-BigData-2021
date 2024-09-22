#2023/4/21
#大数据分析实验三
#APriori算法、PCY算法
#大数据2101班 李嘉鹏 U202115652

import pandas as pd
import numpy as np
def data_set():
    data = pd.read_csv("Groceries.csv")
    col_2 = data['items']
    data = np.array(col_2)
    list_t1 = []
    for line in data:
        line = line.strip('{').strip('}').split(',')
        s = []
        for i in line:
            s.append(i)
        list_t1.append(s)
    data = list_t1
    return data

def Create_C1(data):
    c1 = set()
    for items in data:
        for item in items:
            item_set = frozenset([item])
            c1.add(item_set)
    print('C1共 %s 项' % len(c1))
    print('\n\n\n')
    # print(c1)
    return c1

def is_apriori(ck_item, Lk): #在从Lk构造CK+1的过程中，判定一个项集ck_item是否在Ck+1中
    # 原理：Ck+1中每一个集合的子集都在Lk中
    for item in ck_item:
        sub_item = ck_item - frozenset([item])
        if sub_item not in Lk:
            return False
    return True

def Create_Ck(Lk, k):  # 通过合并Lk-1中的项，获得Ck中的项
    Ck = set()
    len_Lk = len(Lk)
    list_Lk = list(Lk)
    for i in range(len_Lk):
        for j in range(i + 1, len_Lk):
            l1 = list(list_Lk[i])[0:k - 2]
            l2 = list(list_Lk[j])[0:k - 2]
            l1.sort()
            l2.sort()
            if l1 == l2:
                Ck_item = list_Lk[i] | list_Lk[j]
                if is_apriori(Ck_item, Lk): #如果只含k-1个元素或含有大于k+1个元素，则会被直接筛掉
                    Ck.add(Ck_item)
    return Ck

def get_Lk(data_set, Ck, min_support, support_data):
    Lk = set()
    item_count = {}
    for t in data_set:
        for item in Ck:
            if item.issubset(t):
                if item not in item_count:
                    item_count[item] = 1
                else:
                    item_count[item] += 1
    data_num = float(len(data_set))
    for item in item_count:
        if (item_count[item] / data_num) >= min_support: #标准化并与最小支持度比较
            support_data[item] = item_count[item] / data_num
            if(support_data[item]>=min_support): # 只有能满足support大于最小支持度才能加入Lk
                Lk.add(item)
    return Lk

def get_Rule(L, support_data, min_confidence):#获取关系规则
    # 参数：所有的频繁项目集（L1~L3），项目集-支持度dic，最小置信度
    rule_list = []
    sub_set_list = []
    for i in range(len(L)):
        for frequent_set in L[i]:
            for sub_set in sub_set_list:
                if sub_set.issubset(frequent_set):       # 寻找上一层循环中出现的frequent_set的子集
                    conf = support_data[frequent_set] / support_data[sub_set]  # conf(rule)=S(J)/S(J-j)
                    rule = (sub_set, frequent_set - sub_set, conf)
                    if conf >= min_confidence and rule not in rule_list: #找出大于最小置信度的规则
                        rule_list.append(rule)
            sub_set_list.append(frequent_set)
    return rule_list

if __name__ == "__main__":
    data = data_set()
    minsupport = 0.005
    minconfidence = 0.5
    support_data = {}
    C1 = Create_C1(data)
    L1 = get_Lk(data, C1, minsupport, support_data)
    print('L1共 %d 项' % (len(L1)))
    Lk = L1.copy()
    for key in Lk:
        print(key, 'support:', support_data[key])
    print('\n\n\n')
    L = [] #记录L1~L3
    L.append(Lk)
    for k in range(2, 4):
        Ck = Create_Ck(Lk, k)
        print('C%d共 %d 项' %(k,len(Ck)))
        # print(Ck)
        Lk = get_Lk(data, Ck, minsupport, support_data)
        print('L%d共 %d 项：' % (k, len(Lk)))
        # print(Lk)

        Lk = Lk.copy()
        L.append(Lk)
        for key in Lk:
            print(key, 'support:', support_data[key])
        print('\n\n\n')
    rule_list = get_Rule(L, support_data, minconfidence)
    print('关联规则总数：%d' % (len(rule_list)))
    for item in rule_list:
        print(item[0], '--->', item[1], 'confidence:', item[2])
    with open('L1.csv', 'w') as f:
        for key in L[0]:
            f.write('{},\t{}\n'.format(key, support_data[key]))
    with open('L2.csv', 'w') as f:
        for key in L[1]:
            f.write('{},\t{}\n'.format(key, support_data[key]))
    with open('L3.csv', 'w') as f:
        for key in L[2]:
            f.write('{},\t{}\n'.format(key, support_data[key]))
    with open('rule.csv', 'w') as f:
        for item in rule_list:
            f.write('{}\t{}\t{}\t: {}\n'.format(item[0], "of", item[1], item[2]))