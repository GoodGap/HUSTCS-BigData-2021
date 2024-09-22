# 大数据管理实验代码

## Lab 2 MongoDB

1.

db.review.find().limit(2).skip(6)

2.

db.business.find({'city':'Las Vegas'}).limit(5)

3.

db.user.find({'name':'Steve'},{'useful':1,'cool':1}).limit(10)

4.

db.user.find({'funny':{$in:[66,67,68]}},{'name':1,'funny':1}).limit(20)

5.

db.user.find({'cool':{$gte:15,$lt:20},'useful':{$gte:50}}).limit(10)

6.

db.business.aggregate({$group:{_id:'',total:{$sum:1}}})

db.business.explain("executionStats").count()

7.

db.business.find({'city':{$in:['Westlake','Calgary']}})

8.

db.business.find({'categories':{$size:6}},{'_id':1,'categories':1}).limit(10)

9.

db.business.find({ business_id: "5JucpCfHZltJh5r1JabjDg" }).explain("executionStats")

10.

db.business.aggregate([

{$group:{cnt:{$sum:1}, '_id':'$stars'}},

{$sort:{'_id': -1}}])

11.

db.review.aggregate([{$limit: 500000}, {$out: "Subreview"}])

db.Subreview.createIndex({'text':'text'})

db.Subreview.createIndex({'useful':1})

db.Subreview.find({'useful':{$gt:9}, '$text':{$search:'delicious'}})

12.

db.Subreview.aggregate([ 
{$match:{'useful':{$gt:6}, 'funny':{$gt:6}, 'cool':{$gt:6}}}, 
{$group:{_id:'$business_id', stars_avg:{$avg:'$stars'}}}, 
{$sort:{_id:-1}} 
])

统计结果数量：

db.Subreview.aggregate([ 
{$match:{'useful':{$gt:6}, 'funny':{$gt:6}, 'cool':{$gt:6}}}, 
{$group:{_id:'$business_id', stars_avg:{$avg:'$stars'}}}, 
{$sort:{_id:-1}}, 

{$count: "TotalCount"}

])

13.

db.business.createIndex({'loc':"2dsphere"})

查询目标商家的坐标：

db.business.find(
{loc: {$near:{$geometry:{type:"Point", coordinates:[-112.3955963552, 33.4556129678]}, $maxDistance: 100}}}, 

{'name':1, 'address':1, 'stars':1, '_id':0}

)

14.

首先创建一个关于用户id和评论日期的索引db.Subreview.createIndex({user_id: 1, date: 1})

然后聚合查询

db.Subreview.aggregate([

{$match:{'date':{$gte: "2017-01-01"}}}, 

{$group:{_id:"$user_id", total:{$sum:1}}}, 

{$sort: {total: -1}}, 

{$limit: 20}

])

15.

db.business.mapReduce(

function(){

emit(this.business_id, {stars: this.stars, count:1});

},



function(key, values){

var totalStars=0;

var totalCount=0;

values.forEach(function(value) {

totalStars+=value.stars;

totalCount+=value.count;

});

return {stars:totalStars, count:totalCount};

},

{

out:"test_map_reduce",

finalize: function(key, reducedValue){

var avgStars = reducedValue.stars / reducedValue.count;

return { count: reducedValue.count, stars: reducedValue.stars, avg: avgStars };

}

}

)

## Lab 3 Neo4j

1.

MATCH (city:CityNode)
RETURN city
LIMIT 10

2.

MATCH (business:BusinessNode {city:'Ambridge'}
RETURN business

3.

MATCH (r:ReviewNode {reviewid:'rEITo90tpyKmEfNDp3Ou3A'})-[:Reviewed]->(business:BusinessNode)
RETURN business

4.

MATCH (user:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(:BusinessNode {businessid:'fyJAqmweGm8VXnpU4CWGNw'})
RETURN user.name, user.fans

5.

MATCH (:UserNode {userid:'TEtzbpgA2BFBrC0y0sCbfw'})-[:Review]->(:ReviewNode {stars:'5.0'})-[:Reviewed]->(business:BusinessNode)
RETURN business.name, business.address

6.

MATCH (business:BusinessNode)
RETURN business.name, business.stars, business.address
ORDER BY business.stars DESC
LIMIT 15

7.

MATCH (user:UserNode)
WHERE tointeger(user.fans)>200
RETURN user.name, user.fans
LIMIT 10

8.

MATCH (:BusinessNode {businessid:'tyjquHslrAuF5EUejbPfrw'})-[:IN_CATEGORY]->(c:CategoryNode)
RETURN count(c)

9.

MATCH (business:BusinessNode {businessid:'tyjquHslrAuF5EUejbPfrw'})-[:IN_CATEGORY]->(c:CategoryNode)
RETURN collect(c.category) as category

10.

MATCH(:UserNode {name:'Allison'})-[:HasFriend]->(friend:UserNode)
WITH friend.name as friendsList, size((friend) -[:HasFriend]->()) as numofFOFs
RETURN friendsList, numofFOFs

11.

MATCH (business:BusinessNode)-[:IN_CATEGORY]->(:CategoryNode {category:'Salad'})
WITH business.city as city, count(*) as count
RETURN city, count
ORDER BY count DESC
LIMIT 5

12.

MATCH (business:BusinessNode)
WITH business.name as name, count(*) as cnt
RETURN name, cnt
ORDER BY cnt DESC
LIMIT 10

13.

MATCH (businessall:BusinessNode)
WITH count(distinct businessall.name) as cnt
MATCH (business:BusinessNode)
WHERE tointeger(business.reviewcount) > 5000
WITH cnt, count(business.name) as subcnt, business.name as name, business.reviewcount as reviewcount
RETURN subcnt*1.0/cnt, name, reviewcount
ORDER BY reviewcount DESC

14.

MATCH (business:BusinessNode)-[:IN_CATEGORY]->(:CategoryNode {category:'Zoos'})
WITH business
MATCH (:ReviewNode {stars:'5.0'})-[:Reviewed]->(business:BusinessNode)
RETURN distinct business.city

15.

MATCH (user:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(business:BusinessNode)
WITH count(distinct user) as user_count, business
RETURN business.businessid, business.name, user_count
ORDER BY user_count DESC
LIMIT 10

16.

准备工作，使用语句为UserNode新建一个属性flag用来实验

MATCH (user:UserNode)

SET user.flag = user.fans

重置属性信息（最上面的步骤），然后为UserNode的属性flag建立索引

CREATE INDEX FOR (user:UserNode) ON (user.flag)

17.

MATCH (user1:UserNode {userid: 'tvZKPah2u9G9dFBg5GT0eg'})-[:Review]->(:ReviewNode)-[:Reviewed]->(b:BusinessNode)
WITH user1, COLLECT(DISTINCT b) AS user1_businesses
MATCH (user2:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(b:BusinessNode)
WHERE NOT EXISTS ((user1)-[:HasFriend]->(user2)) AND b IN user1_businesses AND user1<>user2
WITH user1, user2, COUNT(DISTINCT b) AS num
RETURN user1.name, user2.name, num
ORDER BY num DESC