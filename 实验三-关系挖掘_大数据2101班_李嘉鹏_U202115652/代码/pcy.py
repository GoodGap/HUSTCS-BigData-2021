#2023/4/21
#大数据分析实验三
#APriori算法、PCY算法（Multistage）
#大数据2101班 李嘉鹏 U202115652

import pandas as pd
import numpy as np
import mmh3

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
            c1.add(item)
    c1 = list(c1)
    return c1

# 计算每个候选项集的支持度
def get_support(data, candidates, min_support):
    supports = {}
    for candidate in candidates:
        count = 0
        for transaction in data:
            if set(candidate).issubset(set(transaction)):
                count += 1
        support = count / len(data)
        if support >= min_support:
            supports[tuple(candidate)] = support #若大于最小支持度，则赋值为相应的支持度；否则支持度等于0
    return supports

# 生成下一阶候选项集
def generate_candidates(items, k):
    candidates = set()
    for i in range(len(items)):
        for j in range(i + 1, len(items)):
            # 连接
            union_item = sorted(list(set(items[i]) | set(items[j])))
            if len(union_item) == k: #元素数量为k才进入下一步检查
                flag = True
                for item in union_item:
                    sub_item = set(union_item) - set([item])
                    sub_item = set([tuple(sorted(list(sub_item)))])
                    if not sub_item.issubset(set(items)):
                        flag = False
                        break
                if flag:
                    candidates.add(tuple(union_item))
    return list(candidates)

# 获取所有频繁项集
def get_frequent_itemsets_pcy(data, min_support, hash_size):
    frequent_itemsets = []
    itemsets = []
    c1 = Create_C1(data)
    c1 = sorted(c1)
    c1 = [tuple([word]) for word in c1]
    l1 = get_support(data, c1, min_support)
    frequent_itemsets.append(l1)
    itemsets.append(list(l1.keys()))
    k = 2
    while k <= 3:
        candidates = list(l1.keys())
        if k == 2: #用Multihash对二阶频繁项集的计算阶段进行优化
            # 第一次哈希，使用mmh3.hash函数
            hash_table1 = [0] * hash_size
            for transaction in data:
                for itemset in candidates:
                    if set(itemset).issubset(set(transaction)):
                        hash_value = abs(mmh3.hash(str(tuple(itemset)))) % hash_size
                        #将itemset转变成字符串并获取哈希值
                        hash_table1[hash_value] += 1
            filtered_candidates1 = [] #记录可能为频繁项集的candidates
            for itemset in candidates:
                hash_value = abs(mmh3.hash(str(tuple(itemset)),seed=12345)) % hash_size
                #加入seed，但hash的效果没有区别
                if hash_table1[hash_value]/len(data) >= min_support: #标准化
                    filtered_candidates1.append(itemset)
                    candidates = filtered_candidates1 #更新candidates
                else:
                    candidates = sorted(candidates) #保持不变
            # 第二次哈希，使用hash函数
            hash_table2 = [0] * hash_size
            for transaction in data:
                for itemset in candidates:
                    if set(itemset).issubset(set(transaction)):
                        hash_value = hash(tuple(itemset)) % hash_size
                        hash_table2[hash_value] += 1
            filtered_candidates2 = []
            for itemset in candidates:
                hash_value = hash(tuple(itemset)) % hash_size
                if hash_table2[hash_value]/len(data) >= min_support:
                    filtered_candidates2.append(itemset)
                    candidates = filtered_candidates2
                else:
                    candidates = sorted(candidates)
        candidates = generate_candidates(itemsets[k - 2], k)
        supports = get_support(data, candidates, min_support)
        frequent_itemsets.append(supports)
        itemsets.append(list(supports.keys()))
        k += 1
    return frequent_itemsets, hash_table1, hash_table2

# 获取所有关联规则
def get_association_rules(frequent_itemsets, min_confidence):
    rules = []
    for itemset in frequent_itemsets[1].keys():
        if len(itemset) > 1:
            for i in range(len(itemset)):
                # 筛选符合最小置信度的规则
                antecedent = tuple(set(itemset) - set([itemset[i]]))
                if frequent_itemsets[1].get(itemset) / frequent_itemsets[0].get(antecedent) >= min_confidence:
                    rules.append((antecedent, [itemset[i]],
                                    frequent_itemsets[1].get(tuple(itemset)) / frequent_itemsets[0].get(antecedent)))

    for itemset in frequent_itemsets[2].keys():
        if len(itemset) > 1:
            for i in range(len(itemset)):
                # 筛选符合最小置信度的规则
                antecedent = tuple(sorted(list(set(itemset) - set([itemset[i]]))))
                if frequent_itemsets[2].get(itemset) / frequent_itemsets[1].get(antecedent) >= min_confidence:
                    rules.append((antecedent, [itemset[i]],
                                    frequent_itemsets[2].get(tuple(itemset)) / frequent_itemsets[1].get(antecedent)))
    return rules

if __name__ == '__main__':
    data = data_set()
    frequent_itemsets, hash_table1, hash_table2 = get_frequent_itemsets_pcy(data, 0.005, 10000)
    association_rules = get_association_rules(frequent_itemsets, 0.5)
    print('Hash_Table_1：', hash_table1)
    print('Hash_Table_2：', hash_table2)
    print('\n\n\n')
    for i in range(1, 4):
        print('L%d共 %d 项：' %(i, len(frequent_itemsets[i-1])))
        for itemset,support in frequent_itemsets[i-1].items():
            print(list(itemset), "support:", support)
        print('\n\n\n')
    print('关联规则总数：', len(association_rules))
    for rule in association_rules:
        print(list(rule[0]), "--->", list(rule[1]), 'confidence:', rule[2])