# 大数据管理实验代码

## 大数据2101班  李嘉鹏  U202115652

### Lab 1

1.

select *

from business

where business_info->'$.city'="Tampa"

order by business_info->'$.review_count' desc

limit 10;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image002.jpg)

2.

select

json_keys(business_info) as keys_info, 

json_length(business_info) as key_num_info,

json_keys(business_info, '$.attributes') as keys_attr, 

json_length(business_info, '$.attributes') as key_num_attr

from business

limit 5;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image004.jpg)

3.

select 

business_info->'$.name' as name, 

json_type(business_info->'$.name') as name_type, 

business_info->'$.stars' as stars, 

json_type(business_info->'$.stars') as stars_type, 

business_info->'$.attributes' as attributes,

json_type(business_info->'$. attributes') as attributes_type 

from business 

limit 5;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image006.jpg)

4.

select 

json_unquote(business_info->'$.name') as name, 

business_info->'$.attributes' as attributes, 

business_info->'$.hours' as open_time 

from business 

where business_info->'$.attributes.HasTV'="True"

and (business_info->'$.hours' is null or business_info->'$.hours.Sunday' is null) 

order by name 

limit 10;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image008.jpg)

5.

explain format=json 

select * from user where user_info->'$.name'='Wanda';

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image010.jpg)

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg)

使用MongoDB查询同样的语句，并使用explain查看查询计划：

db.user.find({'name':"Wanda"}).explain("executionStats")

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image014.jpg)

6.

select json_pretty(business_info) 

from business 

where business_id="4r3Ck65DCG1T6gpWodPyrg";

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)

更新操作：

update business 

set business_info=json_set(business_info, '$.hours.Tuesday', "16:0-23:0", '$.stars', 4.5, '$.WiFi', "Free") 

where business_id="4r3Ck65DCG1T6gpWodPyrg";

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image020.jpg)

再次查询：

select json_pretty(business_info) 

from business 

where business_id="4r3Ck65DCG1T6gpWodPyrg";

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image022.jpg)

7.

insert into business(business_id, business_info) 

select "aaaaaabbbbbbcccccc2023", business_info from business where business_id='5d-fkQteaqO6CSCqS5q4rw';

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image024.jpg)

update business 

set business_info=json_remove(business_info, '$.name') 

where business_id="aaaaaabbbbbbcccccc2023";

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image026.jpg)

查询：

select json_pretty(business_info) 

from business 

where business_id="aaaaaabbbbbbcccccc2023";

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image028.jpg)

8.

select 

state, 

json_objectagg(city, count) as city_occ_num

from 

( 

select 

business_info->'$.state' as state, 

json_unquote(business_info->'$.city') as city, 

count(*) as count 

from business 

group by state, city 

) as sub 

group by state 

order by state;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image030.jpg)

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image032.jpg)

9.

select sub.userid as user_id, 

uu.user_info->'$.name' as name, 

sub.textarray as text_array from 

(

select t.user_id as userid, 

json_arrayagg(t.tip_info->'$.text') as textarray 

from tip t 

join user u on REGEXP_LIKE(u.user_info->'$.friends', t.user_id) 

where u.user_id='__1cb6cwl3uAbMTK3xaGbg' 

group by t.user_id

) as sub, user uu 

where uu.user_id = sub.userid 

order by name;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image034.jpg)

10.

select 

a.business_info->'$.name' as name1, 

a.business_info->'$.city' as city1, 

b.business_info->'$.name' as name2, 

b.business_info->'$.city' as city2,

a.business_info->'$.hours' as hours1,

b.business_info->'$.hours' as hours2,

JSON_OVERLAPS(a.business_info->'$.hours', b.business_info->'$.hours') as has_same_opentime 

from business a 

join business b 

on a.business_info->'$.city' = 'EdMonton' and b.business_info->'$.city' = 'Elsmere';

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image036.jpg)

11.

select 

user_info->'$.name' as name, 

user_info->'$.average_stars' as avg_stars, 

JSON_ARRAY(user_info->'$.funny', user_info->'$.useful', user_info->'$.cool', user_info->'$.funny'+user_info->'$.useful'+user_info->'$.cool') as '[funny,useful,cool,sum]' 

from user 

where user_info->'$.funny'>2000 

and user_info->'$.average_stars'>4.0 

order by user_info->'$.average_stars' desc

limit 10;

报错：

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image038.jpg)

手动修改缓存池大小即可

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image040.jpg)

在前面加上explain format=json

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image042.jpg)

和第一题对比：

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image044.jpg)

12.

select

json_merge_preserve(b.business_info, u.user_info)

from

(

select business_id, 

count(*) as count

from tip

group by business_id

order by count desc

limit 1

) as tb

join

(

select user_id, 

count(*) as count

from tip

group by user_id

order by count desc

limit 1

) as tu

join business b on tb.business_id=b.business_id

join user u on tu.user_id=u.user_id;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image046.jpg)

13.

select 

  sub.business_name,

  sub.review_count as business_review_count,

case when sub.hours->'$.Tuesday' is null then 0 else 1 end as business_open_on_Tuesday,

  jt.time_slot

from 

  (select

​     business_info->'$.name' as business_name, 

​     business_info->'$.review_count' as review_count,

​     business_info->'$.hours' as hours

   from business

   order by business_info->'$.review_count' desc 

   limit 3) as sub

join json_table(

​    sub.hours,

​    "$.*" columns (

​      time_slot VARCHAR(255) PATH '$')

  ) as jt on 1=1

order by sub.business_name;

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image048.jpg)



## Lab 2

1.

db.review.find().limit(2).skip(6)

![image-20231017212035757](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231017212035757.png)

2.

db.business.find({'city':'Las Vegas'}).limit(5)

![image-20231017212538942](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231017212538942.png)

3.

db.user.find({'name':'Steve'},{'useful':1,'cool':1}).limit(10)

![image-20231017221852064](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231017221852064.png)

4.

db.user.find({'funny':{$in:[66,67,68]}},{'name':1,'funny':1}).limit(20)

![image-20231017222108267](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231017222108267.png)

5.

db.user.find({'cool':{$gte:15,$lt:20},'useful':{$gte:50}}).limit(10)

![image-20231017225411057](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231017225411057.png)

6.

db.business.aggregate({$group:{_id:'',total:{$sum:1}}})

![image-20231017235817389](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231017235817389.png)

db.business.explain("executionStats").count()

![image-20231018103723839](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018103723839.png)

7.

db.business.find({'city':{$in:['Westlake','Calgary']}})

![image-20231018104305864](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018104305864.png)

![image-20231018104440393](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018104440393.png)

8.

db.business.find({'categories':{$size:6}},{'_id':1,'categories':1}).limit(10)

![image-20231018104929478](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018104929478.png)

9.

db.business.find({ business_id: "5JucpCfHZltJh5r1JabjDg" }).explain("executionStats")

![image-20231018105236007](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018105236007.png)

可以看出使用了COLLSCAN方式（遍历整个集合）

执行计划关注parsedQuery（解析的查询表达式）、winningPlan（查询执行策略）、rejectedPlans（考虑但未选中的执行策略）

通过物理方式进行优化：为business_id建立索引。

db.business.createIndex({'business_id':1})

![image-20231018112131195](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018112131195.png)

再次使用explain查看执行过程：

![image-20231018112451886](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018112451886.png)

![image-20231018112518962](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018112518962.png)

很明显可以看到时间变短（64ms->0ms），且查询方式变为FETCH，索引名为business_id_1

10.

db.business.aggregate([

{$group:{cnt:{$sum:1}, '_id':'$stars'}},

{$sort:{'_id': -1}}])

![image-20231018114717948](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018114717948.png)

11.

db.review.aggregate([{$limit: 500000}, {$out: "Subreview"}])

![image-20231018123505993](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018123505993.png)

db.Subreview.createIndex({'text':'text'})

![image-20231018124624116](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018124624116.png)

db.Subreview.createIndex({'useful':1})

![image-20231018124652645](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018124652645.png)

db.Subreview.find({'useful':{$gt:9}, '$text':{$search:'delicious'}})

![image-20231018125928000](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018125928000.png)

![image-20231018125311411](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018125311411.png)

12.

db.Subreview.aggregate([ 
{$match:{'useful':{$gt:6}, 'funny':{$gt:6}, 'cool':{$gt:6}}}, 
{$group:{_id:'$business_id', stars_avg:{$avg:'$stars'}}}, 
{$sort:{_id:-1}} 
])

![image-20231018133433884](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018133433884.png)

统计结果数量：

db.Subreview.aggregate([ 
{$match:{'useful':{$gt:6}, 'funny':{$gt:6}, 'cool':{$gt:6}}}, 
{$group:{_id:'$business_id', stars_avg:{$avg:'$stars'}}}, 
{$sort:{_id:-1}}, 

{$count: "TotalCount"}

])

![image-20231018173130083](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018173130083.png)

13.

db.business.createIndex({'loc':"2dsphere"})

![image-20231018174024830](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018174024830.png)

查询目标商家的坐标：

![image-20231018175008397](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018175008397.png)

db.business.find(
{loc: {$near:{$geometry:{type:"Point", coordinates:[-112.3955963552, 33.4556129678]}, $maxDistance: 100}}}, 

{'name':1, 'address':1, 'stars':1, '_id':0}

)

![image-20231018175618030](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018175618030.png)

14.

首先创建一个关于用户id和评论日期的索引

db.Subreview.createIndex({user_id: 1, date: 1})

然后聚合查询

db.Subreview.aggregate([

{$match:{'date':{$gte: "2017-01-01"}}}, 

{$group:{_id:"$user_id", total:{$sum:1}}}, 

{$sort: {total: -1}}, 

{$limit: 20}

])

![image-20231018185538517](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018185538517.png)

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

![image-20231018191345689](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018191345689.png)

![image-20231018191407071](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018191407071.png)

![image-20231018191642066](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231018191642066.png)



## Lab 3

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

![17前](C:\Users\xiaoy\Downloads\17前.svg)

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

![17后](C:\Users\xiaoy\Downloads\17后.svg)

18.

Neo4j查询：

MATCH (:ReviewNode {reviewid:'TIYgnDzezfeEnVeu9jHeEw'})-[:Reviewed]->(business:BusinessNode)
RETURN business

![image-20231026105836418](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026105836418.png)

MongoDB查询：

var bid=db.review.findOne({"review_id":"TIYgnDzezfeEnVeu9jHeEw"}).business_id

db.business.findOne({"business_id":bid})

![image-20231026131856916](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231026131856916.png)

MongoDB查询所用时间略大于Neo4j，据此可以指出Neo4j和MongoDB主要的适用场景。

Neo4j适用于复杂的图结构数据查询，如社交网络、推荐系统、知识图谱等。由于其使用基于图论的查询语言Cypher，支持关系型数据的快速查询和分析。

而MongoDB适用于海量非结构化或半结构化数据存储和查询，如日志、传感器数据、文档数据库等。由于其使用文档数据库模型，支持高效的数据插入和查询，以及分布式数据库集群的横向扩展。



## Lab 4

1.

MATCH (user:UserNode)-[:Review]->(:ReviewNode)-[:Reviewed]->(business:BusinessNode)
WITH user, COUNT(distinct business) as count
WHERE count>5
RETURN user.name, user.funny, user.fans, count

![image-20231027104209119](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027104209119.png)

2.

将1得到的结果导入MongoDB，并使用该表格数据，统计其中所有出现的用户名及该用户名对应的出现次数，并按照出现次数降序排序,使用aggregate实现

1）从Neo4j的查询中导出csv文件（export.csv）

2）在mongodb新建集合from_neo4j，将csv文件导入集合

C:\GAP\大数据管理实验>scp ./export.csv root@1.94.55.43:/root/

![image-20231027111809504](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027111809504.png)

![image-20231027112602398](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027112602398.png)

然后启动mongo，选择yelp数据库，创建一个新的集合from_neo4j

db.createCollection("from_neo4j")

![image-20231027112818916](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027112818916.png)

使用show collections查看当前集合：

![image-20231027112909900](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027112909900.png)

退出mongoDB，回到主目录，把数据导入到mongoDB中yelp数据集的from_neo4j集合中。

mongoimport -d=yelp -c=from_neo4j --type=csv --headerline ./export.csv

![image-20231027113553823](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027113553823.png)

3）统计其中所有出现的用户名及该用户名对应的出现次数，并按照出现次数降序排序。

db.from_neo4j.aggregate([
{$group:{_id:'$u.name', count:{$sum:1}}},
{$sort:{count: -1}}
])

![image-20231027115250096](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027115250096.png)

3.

MATCH (business:BusinessNode)-[:IN_CATEGORY]->(c:CategoryNode)
RETURN business.name as name, business.city as city, c.category as category

![image-20231027124011414](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027124011414.png)

然后类似于上面第2题的步骤：

![image-20231027125229570](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027125229570.png)

db.createCollection("AllBusiness")

![image-20231027153840665](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027153840665.png)

退出mongoDB，回到主目录，把数据导入到mongoDB中yelp数据集的AllBusiness集合中。

mongoimport -d=yelp -c=AllBusiness --type=csv --headerline ./AllBusiness.csv

![image-20231027154017746](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027154017746.png)

接下来使用aggregate对AllBusiness去重，仅保留城市、商铺类型。首先创建一个集合用于保存结果

db.createCollection("DistinctBusiness")

db.AllBusiness.aggregate([{ $group: { _id: { city: '$city', category: '$category' } } } ]).forEach((item) => { db.DistinctBusiness.insert( item._id ) } )

查看结果：

![image-20231027154756595](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027154756595.png)

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

![image-20231027174435812](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027174435812.png)

最后查看City-[Has]->Category图谱

MATCH p=()-[r:Has]->()

RETURN p

LIMIT 20

![graph](C:\Users\xiaoy\Downloads\graph.svg)



## Lab 5

1.

体验 MySQL 在 InnoDB 存储引擎下的 MVCC 多版本并发控制，实现的事务 ACID 特性。请注意 Mysql 需要选用什么事务隔离级来支持 MVCC？请构造多用户多写多读案例来展现 MVCC 并发控制特性，解释各种结果产生的原因。

在MySQL中，首先创建一个数据库testdb

mysql -u root -p

create database testdb;
use testdb;

![image-20231027210001069](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027210001069.png)

在testdb中创建一个新表，设置engine=InnoDB

create table test (
 id int(10) not null,
 name varchar(20) not null,
 flag int(5) not null,
 primary key(id)
) engine=InnoDB;

设置事务隔离等级为可重复读（repeatable read）

set session transaction isolation level repeatable read;

插入初始数据：

insert into test VALUES(1, 'LJP', 27);

insert into test VALUES(2, 'DJE', 42);

insert into test VALUES(3, 'OFW', 61);

insert into test VALUES(4, 'SLX', 15);

使用select * from test;查看当前表内的数据：

![image-20231027211143032](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027211143032.png)

开始一个事务：

start transaction;

然后打开会话窗口B（直接在Xshell中完成）。窗口A将id为3的flag更新为88，与此同时，在另外一边，窗口B将id为3的flag更新为99。

start transaction;

update test set flag=88 where id=3; //窗口A

update test set flag=99 where id=3; //窗口B  

在窗口A中查询当前表内的数据，可以发现A对id为3的数据更新成功，在本地查询的flag为88。但是对于窗口B，其对id为3的数据的更新操作受到阻塞（“Query execution was interrupted”），且在二次查询时发现该数据的flag仍然为A更新之前的值，也就是说在窗口A对数据的更新操作提交（commit）之前，会屏蔽窗口B对同一数据的更新操作，同时由于MVCC的特性，窗口B在查询数据时仍然会使用本地保存的版本。

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image002.jpg)

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image002.jpg)

现在，再在窗口A中完成提交commit操作，到窗口B中查询当前数据，发现此时的数据变为最新版本。此时窗口B可以正常对id为3的数据进行更新：

![img](file:///C:/Users/xiaoy/AppData/Local/Temp/msohtmlclip1/01/clip_image002.jpg)

2.

体验 MongoDB 的 MVCC，数据集可自建或选用 yelp 数据集中的 test 集合中进行测试，测试方法同 MySQL。请对测试结果进行说明，并与 MySQL 的 MVCC 实验结果进行对比分析。建议创建 MongoDB 副本或分片集群，体验 MVCC 的不同效果（可选做其一）。

![image-20231030201847235](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030201847235.png)

创建三台服务器(test-0001 test-0002 test-0003)，弹性公网ip分别为：

60.204.242.167

60.204.228.189

60.204.244.116

添加入方向和出方向规则，检验是否可以ping通：

![image-20231030173616390](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030173616390.png)

三台服务器分别创建数据存放的目录：

![image-20231030202907668](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030202907668.png)

![image-20231030203055209](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030203055209.png)

![image-20231030203108454](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030203108454.png)

分别完成shard1和shard2的replica set配置：

![image-20231030203431445](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030203431445.png)

![image-20231030203716412](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030203716412.png)

初始化shard1和shard2的replica set：

shard1，在某一台服务器上执行：

mongo 60.204.242.167:27017

config = { _id: 'shard1', members: [ {_id: 0, host: '60.204.242.167:27017'}, {_id: 1, host: '60.204.228.189:27017'}, {_id: 2, host: '60.204.244.116:27017'}] }

rs.initiate(config);

shard2，在某一台服务器上执行：

mongo 60.204.242.167:27018

config = { _id: 'shard2', members: [ {_id: 0, host: '60.204.242.167:27018'}, {_id: 1, host: '60.204.228.189:27018'}, {_id: 2, host: '60.204.244.116:27018'}] }

rs.initiate(config);

![image-20231030203625817](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030203625817.png)

![image-20231030203948915](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030203948915.png)

配置config server：

在三台服务器上分别执行：

mongod --configsvr --replSet config --dbpath /usr/local/mongodb/data/config --port 20000 --logpath /usr/local/mongodb/data/config.log --logappend --bind_ip=0.0.0.0 --fork

在某一台服务器上执行：

mongo 60.204.242.167:20000

config = { _id: 'config', members: [ {_id: 0, host: '60.204.242.167:20000'}, {_id: 1, host: '60.204.228.189:20000'}, {_id: 2, host: '60.204.244.116:20000'}] }

rs.initiate(config);

![image-20231030204306314](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030204306314.png)

配置mongos：

在三台服务器上分别执行：

mongos --configdb config/60.204.242.167:20000,60.204.228.189:20000,60.204.244.116:20000 --port 30000 --logpath /usr/local/mongodb/data/mongos.log --logappend --bind_ip=0.0.0.0 --fork

使用mongos：

mongo 60.204.242.167:30000

切换到admin，添加分片：

use admin;

db.runCommand({addshard:"shard1/60.204.242.167:27017,60.204.228.189:27017,60.204.244.116:27017",name:"s1", maxsize:20480});

db.runCommand({addshard:"shard2/60.204.242.167:27018,60.204.228.189:27018,60.204.244.116:27018",name:"s2", maxsize:20480});

![image-20231030190434038](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030190434038.png)

（上图为报错，因为addshard字段内严格不允许出现空格）

![image-20231030204648758](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030204648758.png)

激活数据库分片：

创建一个名为testdb的数据库：

use testdb

![image-20231030191109715](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030191109715.png)

激活分片：

sh.enableSharding("testdb")

![image-20231030204801046](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030204801046.png)

使用sh.status()查看数据库当前情况：

![image-20231030204830940](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030204830940.png)

创建一个新的集合testc：

db.createCollection("testc")

![image-20231030214116931](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030214116931.png)

插入一些数据

db.testc.insertMany([
  {"_id": 1, "name": "LJP", "age": 27},
  {"_id": 2, "name": "DJE", "age": 42},
  {"_id": 3, "name": "OFW", "age": 61},
  {"_id": 4, "name": "SLX", "age": 15}
]);

![image-20231030214328897](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030214328897.png)

通过db.testc.find()可以查看当前集合中的数据：

![image-20231030214407097](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030214407097.png)

此时打开test-0002服务器进入mongos：

mongo 60.204.228.189:30000

查询数据：

db.getSiblingDB("testdb").getCollection("testc").find({});

![image-20231030214912129](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030214912129.png)

删除一条数据：

db.getSiblingDB("testdb").getCollection("testc").deleteOne({ _id: 3 })

![image-20231030215102075](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030215102075.png)

再回到test-0001服务器，通过db.testc.find()查看当前集合中的数据：

![image-20231030215201365](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030215201365.png)

MySQL和mongoDB的MVCC对比分析：

MySQL和MongoDB都实现了MVCC（多版本并发控制）机制，用以解决读写冲突的并发控制。在MVCC机制中，为事务分配单向增长的时间戳，并为每个修改保存一个版本，版本与事务时间戳关联。读操作只会读取该事务开始前的数据库快照，从而避免阻塞其它读操作。

虽然二者都采用了MVCC，但在具体实现上存在显著差异。MySQL的MVCC是通过保存数据的历史版本来实现的，每个数据项都有一个时间戳timestamp，可以追踪到创建和修改的时间点。这种实现方式使得MySQL能够提供严格的可重复读，保证读取操作不会看到未提交（commit）的修改，对已修改但未提交的数据进行更新也是非法的。

MongoDB的MVCC实现更复杂。在MongoDB中，每个文档都有一个包含多个版本的时间戳数组，也可以看作一个topologyVersion（拓扑版本号）。当文档被修改时，旧版本并不会被删除，而是存储在数组中，而在绝大部分情况下读写操作都使用的是当前最新的版本（当然，这种方式也允许MongoDB在读取数据时查看过时的数据版本），从而实现非阻塞读写操作。