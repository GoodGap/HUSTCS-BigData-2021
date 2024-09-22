import math
import numpy as np
import pandas as pd

user_count = 671
def getMovies():
    data = pd.read_csv('movies.csv')    # 读取电影名和标签
    col_1 = data['movieId']
    col_2 = data['title']
    col_3 = data['genres']
    movies = {}
    movies_title = {}
    i = 0
    for line in col_3:
        arr = line.split("|")
        movies[col_1[i]] = arr
        i += 1
    i = 0
    for line in col_2:
        arr = line
        movies_title[col_1[i]] = arr
        i += 1
    return movies, movies_title

def get_rating():     # 读取train_set中的评分信息
    f = open('train_set.csv')
    ratings = f.readlines()
    f.close()
    r = []
    ratings.pop(0)
    for line in ratings:
        rate = line.strip().split(',')
        r.append([int(rate[0]), int(rate[1]), float(rate[2])])
    movies = []
    for x in r:
        if x[1] not in movies:
            movies.append(x[1])
    m = len(movies)
    user_movie = np.zeros([user_count, m])
    for item in r:
        y = movies.index(item[1])
        user_movie[item[0]-1, y] = item[2]  # 生成一个以用户为行、评分为列的矩阵
    user_user = np.corrcoef(user_movie)     # 计算用户的pearson相似度矩阵
    return r, user_user

# 用户字典：user_rate[用户id]=[(电影id,电影评分)...]
# 电影字典：movie_user[电影id]=[用户id1,用户id2...]
def get_user(r):
    user_rate = {}
    movie_user = {}
    for i in r:
        user_rank = (i[1], i[2])
        if i[0] in user_rate:
            user_rate[i[0]].append(user_rank)
        else:
            user_rate[i[0]] = [user_rank]
        if i[1] in movie_user:
            movie_user[i[1]].append(i[0])
        else:
            movie_user[i[1]] = [i[0]]
    return user_rate, movie_user

# 计算与给定用户最相似的k个用户
# 输入：指定的用户ID，用户对电影的评分表，电影对应的用户表；输出：与指定用户最相邻的用户列表
def nearuser_k(userid, user_rate, movie_user, user_user):
    neighbors = []
    neighbors_dist = []
    for item in user_rate[userid]:
        # 在每一部电影与之相关的用户中查找邻居
        for neighbor in movie_user[item[0]]:
            if neighbor != userid and neighbor not in neighbors:
                neighbors.append(neighbor)
                dist = user_user[userid - 1, neighbor - 1]
                neighbors_dist.append([dist, neighbor])
    neighbors_dist.sort(reverse=True)
    return neighbors_dist

def predict_score(userid, movieid, user_rate, movie_user, user_user, k):
    neighbors_dist = nearuser_k(userid, user_rate, movie_user, user_user)
    neighbors_dist = neighbors_dist[:k]
    # 计算相似用户的每一部电影与被推荐用户之间的相似度大小
    sum = 0
    for movie in user_rate[userid]:
        sum += movie[1]
    user_acc = sum / len(user_rate[userid])
    sum2 = 0    # 预测评分
    sum3 = 0    # 相似度之和
    for neighbor in neighbors_dist:
        sum1 = 0
        movies = user_rate[neighbor[1]]  # 相似用户对电影的评分列表
        # 计算每一部电影对用户的推荐程度大小
        for movie in movies:
            if movie[0] == movieid:
                sum1 += neighbor[0]                  # 相似度
                sum2 += neighbor[0] * movie[1]       # 相似度*评分
        if sum1 == 0:
           sum1 = neighbor[0]
           sum2 += neighbor[0] * user_acc   # 当所有相似用户都未对该电影进行评分时，认为用户对其评分为其评分的平均值
        sum3 += sum1
    pred_score = sum2 / sum3
    return pred_score

def recommendation(userid, user_rate, movie_user, user_user, k):
    neighbors_dist = nearuser_k(userid, user_rate, movie_user, user_user)
    neighbors_dist = neighbors_dist[:k]
    # 计算相似用户的每一部电影与被推荐用户之间的预测评分
    recommend_dict = {}
    recommend_movie = {}
    recommend_rating = {}
    sum = 0
    for movie in user_rate[userid]:
        sum += movie[1]
    for neighbor in neighbors_dist:
        movies = user_rate[neighbor[1]]  # 相似用户对电影的评分列表
        sum = 0
        for movie in movies:
            sum += movie[1]
        # 计算每一部电影对该用户的推荐程度大小
        for movie in movies:
            if movie[0] not in recommend_dict:
                recommend_movie[movie[0]] = neighbor[0] * movie[1]
                recommend_dict[movie[0]] = neighbor[0]
            else:
                recommend_movie[movie[0]] += neighbor[0] * movie[1]
                recommend_dict[movie[0]] += neighbor[0]
    # 电影推荐列表
    recommend_list = []
    for key in recommend_dict:
        recommend_rating[key] = recommend_movie[key]/recommend_dict[key]
        recommend_list.append([recommend_dict[key], key])  # 将字典转化为list，其中元素的第一项为推荐程度大小，第二项为电影的ID
    recommend_list.sort(reverse=True)  # 根据推荐的程度大小进行排序
    return recommend_list, recommend_rating

######################### minhash
# def jaccard_similarity(s1, s2):
#     intersection = len(s1 & s2)
#     union = len(s1 | s2)
#     if union == 0:
#         return 0
#     return float(intersection) / union
#
# hash_num = 15          # 降维到15维，使用15个不同的哈希函数
# def minhash_signature(user_rate):
#     signature = np.full(hash_num, np.inf)
#     for i in range(hash_num):
#         for movie_id in user_rate.keys():
#             for user_id in binary_ratings:
#                 minhashvalue = 999999
#                 if binary_ratings[movie_id] == 1:
#                     temphashvalue = ((i-1)*movie_id-1) % 19           # 哈希函数
#                     if temphashvalue < minhashvalue:
#                         minhashvalue = temphashvalue
#                     signature[i] = minhashvalue
#     return signature
##########################

if __name__ == '__main__':
    movies, movies_title = getMovies()
    r, user_user = get_rating()
    user_rate, movie_user = get_user(r)
############################ minhash
    # binary_ratings = {}
    # for user_id in user_rate:
    #     binary_ratings[user_id] = {}
    #     for movie_id in user_rate[user_id]:
    #         if user_rate[user_id][movie_id] < 3:
    #             binary_ratings[user_id][movie_id] = 0
    #         else:
    #             binary_ratings[user_id][movie_id] = 1         # 对效用矩阵作0-1处理
    #
    # minhash_sig = {}
    # for user_id in binary_ratings:
    #     minhash_sig[user_id] = minhash_signature(binary_ratings[user_id])
    #
    # user_user = {}
    # for u1 in minhash_sig:
    #     user_user[u1] = {}
    #     for u2 in minhash_sig:
    #         if u1 == u2:
    #             user_user[u1][u2] = 1
    #         elif u2 in user_user[u1]:
    #             user_user[u1][u2] = user_user[u2][u1]
    #         else:
    #             sim = jaccard_similarity(set(minhash_sig[u1]), set(minhash_sig[u2]))
    #             user_user[u1][u2] = sim
###########################
    print(user_user)
    user_id = int(input('请输入被推荐用户的id（0-运行测试集）:'))
    if user_id != 0:
        movie_id = 1
        k = int(input('请输入相似用户数量k：'))
        n = int(input('请输入推荐电影数量n：'))
        recommend_list, recommend_dict = recommendation(user_id, user_rate, movie_user, user_user, k)
        # 输出前n个推荐项
        print("{0:6}\t{1:6}\t{2:60}\t{3}".format('电影id', '预测评分', '电影名', '电影标签'))
        for item in recommend_list[:n]:
            movie = item[1]
            print("{0:<6}\t{1:.6f}\t{2:<60}\t{3}".format(movie, recommend_dict[movie], movies_title[movie], movies[movie]))
    else:
        data = pd.read_csv('test_set.csv')
        usersid = data['userId']
        moviesid = data['movieId']
        rating = data['rating']
        m = len(usersid)
        k = 50
        sse = 0
        print("{0:4}\t{1:6}\t{2:6}\t{3:6}\t{4}".format('用户id', '电影id', '预测评分', '真实评分', '误差平方'))
        for i in range(m):
            p_rating = predict_score(usersid[i], moviesid[i], user_rate, movie_user, user_user, k)
            print("{0:6}\t{1:<6}\t{2:.6f}\t{3:.6f}\t{4}".format(usersid[i], moviesid[i], p_rating, rating[i], pow(p_rating-rating[i],2)))
            sse += pow(p_rating-rating[i],2)
        print("测试集误差平方和SSE: ", sse)