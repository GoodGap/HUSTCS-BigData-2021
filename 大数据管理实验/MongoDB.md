1.

db.review.find().limit(2).skip(6)

![image-20231017212035757](/大数据管理实验/pic/image-20231017212035757.png)

2.

db.business.find({'city':'Las Vegas'}).limit(5)

![image-20231017212538942](/大数据管理实验/pic/image-20231017212538942.png)

3.

db.user.find({'name':'Steve'},{'useful':1,'cool':1}).limit(10)

![image-20231017221852064](/大数据管理实验/pic/image-20231017221852064.png)



4.

db.user.find({'funny':{$in:[66,67,68]}},{'name':1,'funny':1}).limit(20)

![image-20231017222108267](/大数据管理实验/pic/image-20231017222108267.png)

5.

db.user.find({'cool':{$gte:15,$lt:20},'useful':{$gte:50}}).limit(10)

![image-20231017225411057](/大数据管理实验/pic/image-20231017225411057.png)

6.

db.business.aggregate({$group:{_id:'',total:{$sum:1}}})

![image-20231017235817389](/大数据管理实验/pic/image-20231017235817389.png)



db.business.explain("executionStats").count()

![image-20231018103723839](/大数据管理实验/pic/image-20231018103723839.png)

7.

db.business.find({'city':{$in:['Westlake','Calgary']}})

![image-20231018104305864](/大数据管理实验/pic/image-20231018104305864.png)

![image-20231018104440393](/大数据管理实验/pic/image-20231018104440393.png)

8.

db.business.find({'categories':{$size:6}},{'_id':1,'categories':1}).limit(10)

![image-20231018104929478](/大数据管理实验/pic/image-20231018104929478.png)

9.

db.business.find({ business_id: "5JucpCfHZltJh5r1JabjDg" }).explain("executionStats")

![image-20231018105236007](/大数据管理实验/pic/image-20231018105236007.png)

可以看出使用了COLLSCAN方式（遍历整个集合）

执行计划关注parsedQuery（解析的查询表达式）、winningPlan（查询执行策略）、rejectedPlans（考虑但未选中的执行策略）

通过物理方式进行优化：为business_id建立索引。

db.business.createIndex({'business_id':1})

![image-20231018112131195](/大数据管理实验/pic/image-20231018112131195.png)

再次使用explain查看执行过程：

![image-20231018112451886](/大数据管理实验/pic/image-20231018112451886.png)

![image-20231018112518962](/大数据管理实验/pic/image-20231018112518962.png)

很明显可以看到时间变短（64ms->0ms），且查询方式变为FETCH，索引名为business_id_1

10.

db.business.aggregate([

{$group:{cnt:{$sum:1}, '_id':'$stars'}},

{$sort:{'_id': -1}}])

![image-20231018114717948](/大数据管理实验/pic/image-20231018114717948.png)

11.

db.review.aggregate([{$limit: 500000}, {$out: "Subreview"}])

![image-20231018123505993](/大数据管理实验/pic/image-20231018123505993.png)

db.Subreview.createIndex({'text':'text'})

![image-20231018124624116](/大数据管理实验/pic/image-20231018124624116.png)

db.Subreview.createIndex({'useful':1})

![image-20231018124652645](/大数据管理实验/pic/image-20231018124652645.png)

db.Subreview.find({'useful':{$gt:9}, '$text':{$search:'delicious'}})

![image-20231018125928000](/大数据管理实验/pic/image-20231018125928000.png)

![image-20231018125311411](/大数据管理实验/pic/image-20231018125311411.png)

12.

db.Subreview.aggregate([ 
{$match:{'useful':{$gt:6}, 'funny':{$gt:6}, 'cool':{$gt:6}}}, 
{$group:{_id:'$business_id', stars_avg:{$avg:'$stars'}}}, 
{$sort:{_id:-1}} 
])

![image-20231018133433884](/大数据管理实验/pic/image-20231018133433884.png)

统计结果数量：

db.Subreview.aggregate([ 
{$match:{'useful':{$gt:6}, 'funny':{$gt:6}, 'cool':{$gt:6}}}, 
{$group:{_id:'$business_id', stars_avg:{$avg:'$stars'}}}, 
{$sort:{_id:-1}}, 

{$count: "TotalCount"}

])

![image-20231018173130083](/大数据管理实验/pic/image-20231018173130083.png)

13.

db.business.createIndex({'loc':"2dsphere"})

![image-20231018174024830](/大数据管理实验/pic/image-20231018174024830.png)

查询目标商家的坐标：

![image-20231018175008397](/大数据管理实验/pic/image-20231018175008397.png)

db.business.find(
{loc: {$near:{$geometry:{type:"Point", coordinates:[-112.3955963552, 33.4556129678]}, $maxDistance: 100}}}, 

{'name':1, 'address':1, 'stars':1, '_id':0}

)

![image-20231018175618030](/大数据管理实验/pic/image-20231018175618030.png)

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

![image-20231018185538517](/大数据管理实验/pic/image-20231018185538517.png)



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

![image-20231018191345689](/大数据管理实验/pic/image-20231018191345689.png)

![image-20231018191407071](/大数据管理实验/pic/image-20231018191407071.png)

![image-20231018191642066](/大数据管理实验/pic/image-20231018191642066.png)
