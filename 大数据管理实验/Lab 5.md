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

然后打开另一个会话，直接在xshell中完成即可。

更新id=3的flag为88

update test set flag=88 where id=3;

![image-20231027212117035](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027212117035.png)

在原先的会话中再次使用select * from test;查看当前表内的数据，可以发现已经成功更新：

![image-20231027212154926](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231027212154926.png)



再在第一个会话中更新（先不commit）：update test set flag=100 where id=4;

![image-20231030134019145](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030134019145.png)

到第二个会话中查询当前数据，发现此时无变化

![image-20231030134054717](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030134054717.png)

然后回到第一个终端commit

![image-20231030134123440](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030134123440.png)

![image-20231030134140200](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030134140200.png)



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

MySQL和MongoDB都实现了MVCC（多版本并发控制）机制，用以解决读写冲突的并发控制。在MVCC机制中，为事务分配单向增长的时间戳，并为每个修改保存一个版本，版本与事务时间戳关联。读操作只会读取该事务开始前的数据库快照，从而避免阻塞其他读操作。

虽然二者都采用了MVCC，但在具体实现上存在显著差异。MySQL的MVCC是通过保存数据的历史版本来实现的，每个数据项都有一个时间戳timestamp，可以追踪到创建和修改的时间点。这种实现方式使得MySQL能够提供严格的可重复读，保证读取操作不会看到未提交（commit）的修改。

MongoDB的MVCC实现更复杂。在MongoDB中，每个文档都有一个包含多个版本的时间戳数组，也可以看作一个topologyVersion（拓扑版本号）。当文档被修改时，旧版本并不会被删除，而是存储在数组中。这种方式允许MongoDB在读取数据时查看过时的数据版本，从而实现非阻塞读操作。
