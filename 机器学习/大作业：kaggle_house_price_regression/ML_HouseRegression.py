import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import statistics as st
import missingno as msno
import warnings
warnings.filterwarnings("ignore")

missing_values_set = ["", "--", "?", "na", "NAN", "nan", '']         # 将这些关键字都视为空值（NAN）
test_data = pd.read_csv("./test.csv", na_values=missing_values_set)
train_data = pd.read_csv("./train.csv", na_values=missing_values_set)

# 绘制训练集数值属性相关性示意图
correlation_train = train_data.corr()
mask = np.zeros_like(correlation_train)
mask[np.triu_indices_from(mask)] = True
sns.heatmap(correlation_train, mask=mask, xticklabels=correlation_train.columns, yticklabels=correlation_train.columns, linewidths=0.6, cmap='Blues')
plt.title("训练集中的数值属性相关性示意图")
plt.show()

frames = [train_data, test_data]
full_dataset = pd.concat(frames, sort=False, ignore_index=True)       # 将训练集和测试集合并
full_dataset = pd.DataFrame(data=full_dataset)
SalePrice = full_dataset["SalePrice"]
full_dataset.drop(columns=['SalePrice'])
SalePrice.fillna("", inplace=True)               # 将空值变为""
ID = pd.DataFrame(data=full_dataset["Id"])

# 绘制每列数据的密集程度图（非空）
msno.matrix(full_dataset, labels=True)
plt.xticks(rotation='vertical', fontsize=10)
plt.title("各属性的空值占比（NAN）示意图")
plt.show()

# print(full_dataset.info())

# 数据预处理preprocessing，搜索数据集中空值超过600个的列并删去
many_empty = [attribute for attribute in full_dataset if full_dataset[attribute].isnull().sum() > 600] ##['Alley', 'FireplaceQu', 'PoolQC', 'Fence', 'MiscFeature']
full_dataset = full_dataset.drop(columns=many_empty)

# 获得数值和类别（非数值）属性
numerical_attr = [i for i in full_dataset.select_dtypes(include='number')]
categorical_attr = [i for i in full_dataset.select_dtypes(include='object')]

# 填充剩余列的缺失值。数值属性用这一列的平均值填充，类别属性用这一列出现次数最多的属性填充
for attributes in full_dataset:
    if attributes in numerical_attr:
        full_dataset[attributes] = full_dataset[attributes].fillna(value=full_dataset[attributes].median())
    else:
        full_dataset[attributes] = full_dataset[attributes].fillna(value=full_dataset[attributes].value_counts().idxmax())

# 下面会将类别属性数值化，并利用RFC和SelectFromModel筛选出重要的非数值属性
from sklearn.ensemble import RandomForestClassifier    # 随机森林分类器
from sklearn.feature_selection import SelectFromModel   # 特征选择
from sklearn.model_selection import train_test_split    # 避免overfitting，只关注训练集

X = pd.DataFrame(data=train_data[categorical_attr])
X.drop(columns=['SalePrice'], inplace=True)
# print(X)
Y = train_data['SalePrice']       # X和Y是训练集的属性，其中X不含售价，Y只含售价

for at in X:
    X = pd.concat([X, pd.get_dummies(X[at], prefix=at, drop_first=True)], axis=1, sort=False)     # 利用get_dummies函数将类别属性转化为编码
    X.drop(columns=[at], inplace=True)

X_train_st, X_test_st, y_train_st, y_test_st = train_test_split(X, Y, test_size=0.25)         # 对X和Y分别划分训练集和测试集，比例为3:1

# 特征选择
# 下面首先对类别属性进行操作
rfc = RandomForestClassifier(n_estimators=150)          # 创建150棵树进行随机森林处理
sel = SelectFromModel(estimator=rfc, threshold='mean')  # 利用RFC作为估计器，选取重要度大于全部重要度平均值的属性
sel.fit(X, Y)
print("门限threshold: ", sel.estimator_.feature_importances_.mean())

# print(sel.get_support())                                               # 打印属性是否高于threshold的矩阵（T/F）
selected_features = X_train_st.columns[(sel.get_support())]             # 获取属性名称及取值
print("高于门限值的类别属性取值个数：", len(selected_features), "\n这些属性的值是：")
print(selected_features)

importances = sel.estimator_.feature_importances_               # 获取类别属性特征的重要度并从高到低排序
Features_importance = pd.DataFrame(data=X.columns.values, columns=['Features'])
Features_importance['Importance'] = importances
Features_importance = Features_importance.sort_values(by=['Importance'], axis=0, ascending=False)
print("这些类别属性取值的重要度：")
print(Features_importance)

print("全部类别属性取值的重要度：", importances)
indices = np.argsort(importances)[::-1]                 # 返回需要对数组进行排序的索引

plt.figure(figsize=(10, 10))
plt.title("从高到低排列的类别属性取值的重要度")
plt.bar(range(X.shape[1]), importances[indices], color="r", align="center")
plt.xticks(range(X.shape[1]), Features_importance['Features'], rotation='vertical')
plt.xlim([-1, 9.5])
plt.show()

# 将排名前十的类别属性数值化
cat_to_num = ["GarageFinish", "LotShape", "BsmtExposure", "BsmtFinType1", "LotConfig", "HouseStyle", "MasVnrType"]
for categoricals in cat_to_num:
    full_dataset = pd.concat([full_dataset, pd.get_dummies(full_dataset[categoricals], prefix=categoricals, drop_first=True)], axis=1, sort=False)
    full_dataset.drop(columns=[categoricals], inplace=True)
    print("类别属性：", categoricals)        # 分别将上述类别属性数值化并展示结果
    print(full_dataset)

numerical_attr = [i for i in full_dataset.select_dtypes(include='number')]

# 绘制训练集的完整属性相关性示意图
correlation = full_dataset.corr()
mask = np.zeros_like(correlation)
mask[np.triu_indices_from(mask)] = True
sns.heatmap(correlation, mask=mask, xticklabels=correlation.columns, yticklabels=correlation.columns, linewidths=0.6, cmap='Blues')
plt.title('训练集的完整属性相关性示意图')
plt.show()

# 相关性分析
correlation['CORR'] = correlation.sum(axis=0)                   # 计算每种属性与saleprice的相关系数，并创建一个名为CORR的列
feature_correlation = correlation['CORR'].sort_values(ascending=False)
print("属性相关性分析：\n", feature_correlation)

# 方差分析
vanalysis=[]
for attribute in numerical_attr:
    mean = st.mean(full_dataset[attribute])
    # print('\n head of {} is : {} '.format(attribute, full_dataset[attribute].head()))
    # full_dataset[attribute].value_counts().plot(kind='hist', bins=20)   #绘制每种属性的值与出现频率的直方图
    # plt.title(attribute)
    # plt.show()
    standard_variance = st.stdev(full_dataset[attribute])      # 均值方差标准化
    variance = st.variance(full_dataset[attribute])
    vanalysis.append([attribute, mean, variance, standard_variance])

vanalysis = pd.DataFrame(vanalysis, columns=['属性', '平均值', '方差', '标准化方差']).sort_values(by=['标准化方差'], ascending=False)
print("方差分析：\n", vanalysis)

# 使用Z-score实现数据标准化
from scipy.stats import zscore

old_full_dataset = full_dataset
full_dataset[numerical_attr] = zscore(full_dataset[numerical_attr])        # 对目前的数据集计算zscore

# PCA对数据进行降维处理
from sklearn.decomposition import PCA
pca_search = PCA().fit(full_dataset[numerical_attr])        # fit pca

fig, ax = plt.subplots()
print("PCA的各特征（可解释性）方差贡献率：\n", pca_search.explained_variance_ratio_)
y = np.cumsum(pca_search.explained_variance_ratio_)
xi = np.arange(0, len(y), step=1)                                # 特征数量
plt.plot(xi, y, marker='o', color='b')
plt.xlabel('成分数量')
plt.ylabel('Cumulative variance ratio (%)')      # 累计方差贡献率，取0.72之前的属性
plt.title('The number of components after dimension reduction')
plt.axhline(y=0.72, color='r', linestyle='-')
plt.text(0.5, 0.72, '72% cut-off threshold', color='red', fontsize=12)
ax.grid(axis='x')
plt.show()

data_to_pca = full_dataset.drop(columns=["Id"])         # 舍去id属性
numerical_attr_pca = [i for i in data_to_pca.select_dtypes(include='number')]        # 获取PCA处理后的数值属性

pca = PCA(n_components=24)                # 降维后的维度为24，即保留24种特征
principal_analysis = pca.fit_transform(data_to_pca[numerical_attr_pca])             # 拟合
distortions = []
principal_analysis = pd.DataFrame(data=principal_analysis, columns=['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8', 'P9', 'P10', 'P11', 'P12', 'P13', 'P14', 'P15', 'P16', 'P17', 'P18', 'P19', 'P20', 'P21', 'P22', 'P23', 'P24'])     # 创建dataframe
print("主成分分析（PCA）：\n ", principal_analysis)
print("解释方差比例：", pca.explained_variance_ratio_)

from scipy.spatial.distance import cdist
from sklearn.cluster import KMeans

#计算聚类数量
inertias = []                     # 样本距离最近的聚类中心的总和
distortions = []                  # 每个簇的质点与簇内样本点的平方距离误差和，即畸变程度
K = range(1, 15)
for k in K:
    kmeanModel = KMeans(n_clusters=k).fit(X)
    kmeanModel.fit(X)
    distortions.append(sum(np.min(cdist(X, kmeanModel.cluster_centers_, 'euclidean'), axis=1)) / X.shape[0])
    inertias.append(kmeanModel.inertia_)

# 利用kmeans算法的elbow method原理确定合适的聚类数量
plt.plot(K, distortions, 'bx-')
plt.xlabel('k')
plt.ylabel('Distortion')
plt.title('The Elbow Method')
plt.show()
#elbow with inertias:
# plt.plot(K, inertias, 'bx-')
# plt.xlabel('k')
# plt.ylabel('inertias')
# plt.title('The Elbow Method')
# plt.show()

pca_data = principal_analysis
clustering = KMeans(n_clusters=2, random_state=4)         # 二分类
clustering.fit(pca_data)

predict = clustering.predict(pca_data)
pca_data["Cluster"] = predict                   # 在pca_data中创建三列：聚类（0或1）、Id、售价
pca_data["Id"] = ID.values
pca_data["SalePrice"] = SalePrice.values

cluster_0 = pd.DataFrame(data=pca_data[pca_data['Cluster'] == 0], columns=['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8', 'P9', 'P10', 'P11', 'P12', 'P13', 'P14', 'P15', 'P16', 'P17', 'P18', 'P19', 'P20', 'P21', 'P22', 'P23', 'P24', 'Id', 'Cluster', 'SalePrice'])
cluster_1 = pd.DataFrame(data=pca_data[pca_data['Cluster'] == 1], columns=['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8', 'P9', 'P10', 'P11', 'P12', 'P13', 'P14', 'P15', 'P16', 'P17', 'P18', 'P19', 'P20', 'P21', 'P22', 'P23', 'P24', 'Id', 'Cluster', 'SalePrice'])

print("第一个聚类的数量：", len(cluster_0))
print("第二个聚类的数量：", len(cluster_1))

# 对两个聚类分别进行训练和线性回归
from sklearn.linear_model import LinearRegression
result = []
id = []
clusters = [cluster_0, cluster_1]

for i in range(0, 2):
    train = []
    test = []
    for row in clusters[i].values:
        x = row[26]         # SalePrice
        if x == "":                 # 根据是否已有saleprice，确定每一行进入训练集还是测试集
            test.append(row)
        else:
            train.append(row)

    train_df = pd.DataFrame(train, columns=['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8', 'P9', 'P10', 'P11', 'P12', 'P13', 'P14', 'P15', 'P16', 'P17', 'P18', 'P19', 'P20', 'P21', 'P22', 'P23', 'P24', 'Id', 'Cluster', 'SalePrice'])
    test_df = pd.DataFrame(test, columns=['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8', 'P9', 'P10', 'P11', 'P12', 'P13', 'P14', 'P15', 'P16', 'P17', 'P18', 'P19', 'P20', 'P21', 'P22', 'P23', 'P24', 'Id', 'Cluster', 'SalePrice'])
    Id = pd.DataFrame(data=test_df["Id"])

    train_df.drop(columns=["Cluster"], inplace=True)
    test_df.drop(columns=["Cluster"], inplace=True)

    X_train = train_df.drop(columns=["Id", "SalePrice"])
    Y_train = train_df["SalePrice"]
    X_test = test_df.drop(columns=["Id", "SalePrice"])
    Y_test = test_df["SalePrice"]

    regression = LinearRegression(n_jobs=-1)
    regression.fit(X_train, Y_train)
    prediction = regression.predict(X_test)
    id.append(Id)
    result.append(prediction)

result_for_cluster0 = pd.DataFrame(data=result[0], columns=["cluster_predict"])      # 第一个聚类的预测结果
id0 = id[0]
result_for_cluster0["Id"] = id0.values

result_for_cluster1 = pd.DataFrame(data=result[1], columns=["cluster_predict"])      # 第二个聚类的预测结果
id1 = id[1]
result_for_cluster1["Id"] = id1.values

# 得到结果
saleprice = pd.concat([result_for_cluster0, result_for_cluster1], axis=0, sort=False)
saleprice.sort_values(by=['Id'], inplace=True)
saleprice.rename(columns={"cluster_predict": "SalePrice"}, inplace=True)

saleprice = pd.DataFrame(data=saleprice, columns=["Id", 'SalePrice'])
saleprice.to_csv(r"./saleprice.csv", index=False)