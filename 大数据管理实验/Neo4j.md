1.

MATCH (city:CityNode)
RETURN city
LIMIT 10

![image-20231025092050281](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025092050281.png)

2.

MATCH (business:BusinessNode {city:'Ambridge'}
RETURN business

![image-20231025091959233](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025091959233.png)

3.

MATCH (r:ReviewNode {reviewid:'rEITo90tpyKmEfNDp3Ou3A'})-[:Reviewed]->(business:BusinessNode)
RETURN business

![image-20231025091859571](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025091859571.png)

4.

MATCH (user:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(:BusinessNode {businessid:'fyJAqmweGm8VXnpU4CWGNw'})
RETURN user.name, user.fans

![image-20231025092824071](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025092824071.png)

5.

MATCH (:UserNode {userid:'TEtzbpgA2BFBrC0y0sCbfw'})-[:Review]->(:ReviewNode {stars:'5.0'})-[:Reviewed]->(business:BusinessNode)
RETURN business.name, business.address

![image-20231025183827287](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025183827287.png)

6.

MATCH (business:BusinessNode)
RETURN business.name, business.stars, business.address
ORDER BY business.stars DESC
LIMIT 15

![image-20231025184139300](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025184139300.png)

7.

MATCH (user:UserNode)
WHERE tointeger(user.fans)>200
RETURN user.name, user.fans
LIMIT 10

![image-20231025184502675](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025184502675.png)

8.

MATCH (:BusinessNode {businessid:'tyjquHslrAuF5EUejbPfrw'})-[:IN_CATEGORY]->(c:CategoryNode)
RETURN count(c)

使用PROFILE查看执行计划：

PROFILE MATCH (:BusinessNode {businessid:'tyjquHslrAuF5EUejbPfrw'})-[:IN_CATEGORY]->(c:CategoryNode)
RETURN count(c)

![image-20231025185752386](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025185752386.png)

9.

MATCH (business:BusinessNode {businessid:'tyjquHslrAuF5EUejbPfrw'})-[:IN_CATEGORY]->(c:CategoryNode)
RETURN collect(c.category) as category

![image-20231025190241632](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025190241632.png)

10.

MATCH(:UserNode {name:'Allison'})-[:HasFriend]->(friend:UserNode)
WITH friend.name as friendsList, size((friend) -[:HasFriend]->()) as numofFOFs
RETURN friendsList, numofFOFs

![image-20231025201709618](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025201709618.png)

11.

MATCH (business:BusinessNode)-[:IN_CATEGORY]->(:CategoryNode {category:'Salad'})
WITH business.city as city, count(*) as count
RETURN city, count
ORDER BY count DESC
LIMIT 5

![image-20231025202932077](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025202932077.png)

12.

MATCH (business:BusinessNode)
WITH business.name as name, count(*) as cnt
RETURN name, cnt
ORDER BY cnt DESC
LIMIT 10

![image-20231025203404861](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025203404861.png)

13.

MATCH (businessall:BusinessNode)
WITH count(distinct businessall.name) as cnt
MATCH (business:BusinessNode)
WHERE tointeger(business.reviewcount) > 5000
WITH cnt, count(business.name) as subcnt, business.name as name, business.reviewcount as reviewcount
RETURN subcnt*1.0/cnt, name, reviewcount
ORDER BY reviewcount DESC

![image-20231025205809132](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025205809132.png)

14.

MATCH (business:BusinessNode)-[:IN_CATEGORY]->(:CategoryNode {category:'Zoos'})
WITH business
MATCH (:ReviewNode {stars:'5.0'})-[:Reviewed]->(business:BusinessNode)
RETURN distinct business.city

![image-20231025210313616](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025210313616.png)

15.

MATCH (user:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(business:BusinessNode)
WITH count(distinct user) as user_count, business
RETURN business.businessid, business.name, user_count
ORDER BY user_count DESC
LIMIT 10

![image-20231025211214968](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025211214968.png)

16.

准备工作，使用语句为UserNode新建一个属性flag用来实验

MATCH (user:UserNode)

SET user.flag = user.fans

![image-20231025213800122](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025213800122.png)

![image-20231025214047059](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025214047059.png)

由于服务器内存不够而报错。因此需要对属性作用的范围做出一定的限定，例如fans大于400，于是属性创建完成

MATCH (user:UserNode)

WHERE toInteger(user.fans) > 400

SET user.flag = user.fans

![image-20231025214918792](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025214918792.png)

对UserNode的flag属性执行查询（flag>8000）、创建（flag=10000）、更新（flag=456 to flag=3456）、删除（flag>4000）操作

查询：

MATCH (user:UserNode)
WHERE toInteger(user.flag) > 8000
RETURN user

![image-20231025230046863](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025230046863.png)

![image-20231102170214835](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231102170214835.png)

创建：

MATCH (user:UserNode)
WHERE toInteger(user.fans) = 198
SET user.flag = 10000

![image-20231025233446736](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025233446736.png)

更新：

MATCH (user:UserNode)
WHERE toInteger(user.flag) = 456
SET user.flag = 3456

![image-20231025233749166](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025233749166.png)

删除：

MATCH (user:UserNode)
WHERE user.flag > 4000
REMOVE user.flag

![image-20231025233836760](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025233836760.png)



重置属性信息（最上面的步骤），然后为UserNode的属性flag建立索引

CREATE INDEX FOR (user:UserNode) ON (user.flag)

![image-20231025232431232](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025232431232.png)

对UserNode的flag属性执行相同的查询、创建、更新、删除操作，比较操作时间

查询：

![image-20231025232509864](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025232509864.png)

创建：

![image-20231025231914030](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025231914030.png)

更新：

![image-20231025230254661](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025230254661.png)

删除：

![image-20231025232209484](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231025232209484.png)

17.

MATCH (user1:UserNode {userid: 'tvZKPah2u9G9dFBg5GT0eg'})-[:Review]->(:ReviewNode)-[:Reviewed]->(b:BusinessNode)
WITH user1, COLLECT(DISTINCT b) AS user1_businesses
MATCH (user2:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(b:BusinessNode)
WHERE NOT EXISTS ((user1)-[:HasFriend]->(user2)) AND b IN user1_businesses AND user1<>user2
WITH user1, user2, COUNT(DISTINCT b) AS num
RETURN user1.name, user2.name, num
ORDER BY num DESC

![image-20231101214119756](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231101214119756.png)

![plan](C:\Users\xiaoy\Downloads\plan.svg)

![image-20231103180956214](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231103180956214.png)

优化：

首先，可以为userid和businessid建立索引

CREATE INDEX FOR (user:UserNode) ON (user.userid)

CREATE INDEX FOR (b:BusinessNode) ON (b.businessid)

![image-20231026104943498](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026104943498.png)

![image-20231026105003274](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026105003274.png)

然后将上面的COLLECT函数改为仅对business.businessid聚合，减少内存开销

MATCH (user1:UserNode {userid: 'tvZKPah2u9G9dFBg5GT0eg'})-[:Review]->(:ReviewNode)-[:Reviewed]->(b:BusinessNode)
WITH user1, COLLECT(DISTINCT b.businessid) AS user1_businesses
MATCH (user2:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(b:BusinessNode)
WHERE NOT EXISTS ((user1)-[:HasFriend]->(user2)) AND b.businessid IN user1_businesses AND user1<>user2
WITH user1, user2, COUNT(DISTINCT b) AS num
RETURN user1.name, user2.name, num
ORDER BY num DESC

![image-20231026105037693](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026105037693.png)

![plan (1)](C:\Users\xiaoy\Downloads\plan (1).svg)

18.

Neo4j查询：

MATCH (:ReviewNode {reviewid:'TIYgnDzezfeEnVeu9jHeEw'})-[:Reviewed]->(business:BusinessNode)
RETURN business

![image-20231026105836418](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026105836418.png)

MongoDB查询：

var bid=db.review.findOne({"review_id":"TIYgnDzezfeEnVeu9jHeEw"}).business_id

db.business.findOne({"business_id":bid})

![image-20231026131856916](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026131856916.png)

MongoDB查询所用时间远大于Neo4j，据此可以指出Neo4j和MongoDB主要的适用场景。

Neo4j适用于复杂的图结构数据查询，如社交网络、推荐系统、知识图谱等。由于其使用基于图论的查询语言Cypher，支持关系型数据的快速查询和分析。

而MongoDB适用于海量非结构化或半结构化数据存储和查询，如日志、传感器数据、文档数据库等。由于其使用文档数据库模型，支持高效的数据插入和查询，以及分布式数据库集群的横向扩展。
