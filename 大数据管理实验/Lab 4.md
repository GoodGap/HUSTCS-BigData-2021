1.

MATCH (user:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(business:BusinessNode)
WITH user, COUNT(distinct business) as count
WHERE count>5
RETURN user.name, user.funny, user.fans, count

![image-20231027104209119](/大数据管理实验/pic/image-20231027104209119.png)

2.

将1得到的结果导入MongoDB，并使用该表格数据，统计其中所有出现的用户名及该用户名对应的出现次数，并按照出现次数降序排序,使用aggregate实现

1）从Neo4j的查询中导出csv文件（export.csv）

2）在mongodb新建集合from_neo4j，将csv文件导入集合

C:\GAP\大数据管理实验>scp ./export.csv root@1.94.55.43:/root/

![image-20231027111809504](/大数据管理实验/pic/image-20231027111809504.png)

![image-20231027112602398](/大数据管理实验/pic/image-20231027112602398.png)

然后启动mongo，选择yelp数据库，创建一个新的集合from_neo4j

db.createCollection("from_neo4j")

![image-20231027112818916](/大数据管理实验/pic/image-20231027112818916.png)

使用show collections查看当前集合：

![image-20231027112909900](/大数据管理实验/pic/image-20231027112909900.png)

退出mongoDB，回到主目录，把数据导入到mongoDB中yelp数据集的from_neo4j集合中。

mongoimport -d=yelp -c=from_neo4j --type=csv --headerline ./export.csv

![image-20231027113553823](/大数据管理实验/pic/image-20231027113553823.png)

3）统计其中所有出现的用户名及该用户名对应的出现次数，并按照出现次数降序排序。

db.from_neo4j.aggregate([
{$group:{_id:'$u.name', count:{$sum:1}}},
{$sort:{count: -1}}
])

![image-20231027115250096](/大数据管理实验/pic/image-20231027115250096.png)

3.

MATCH (business:BusinessNode)-[:IN_CATEGORY]->(c:CategoryNode)
RETURN business.name as name, business.city as city, c.category as category

![image-20231027124011414](/大数据管理实验/pic/image-20231027124011414.png)

然后类似于上面第2题的步骤：

![image-20231027125229570](/大数据管理实验/pic/image-20231027125229570.png)

db.createCollection("AllBusiness")

![image-20231027153840665](/大数据管理实验/pic/image-20231027153840665.png)

退出mongoDB，回到主目录，把数据导入到mongoDB中yelp数据集的AllBusiness集合中。

mongoimport -d=yelp -c=AllBusiness --type=csv --headerline ./AllBusiness.csv

![image-20231027154017746](/大数据管理实验/pic/image-20231027154017746.png)

接下来使用aggregate对AllBusiness去重，仅保留城市、商铺类型。首先创建一个集合用于保存结果

db.createCollection("DistinctBusiness")

db.AllBusiness.aggregate([{ $group: { _id: { city: '$city', category: '$category' } } } ]).forEach((item) => { db.DistinctBusiness.insert( item._id ) } )

查看结果：

![image-20231027154756595](/大数据管理实验/pic/image-20231027154756595.png)

将结果导出到服务器主目录下的result.csv中。

mongoexport -d yelp -c DistinctBusiness --type=csv --fields city,category --out result.csv

然后将其放在neo4j安装目录的import下

cd ~/neo4j-community-4.0.9/import

cp /root/result.csv ./

将去重后的结果导入Neo4j中的新库result中，完成（City-[Has]->Category）图谱的构建。

LOAD CSV WITH HEADERS FROM "file:///result.csv" AS f
MERGE (c:CityNode {city: COALESCE(f.city, "")})
MERGE (a:CategoryNode {category: COALESCE(f.category, "")})
CREATE (c) -[:Has]-> (a)

![image-20231027174435812](/大数据管理实验/pic/image-20231027174435812.png)

最后查看City-[Has]->Category图谱

MATCH p=()-[r:Has]->()

RETURN p

LIMIT 20

