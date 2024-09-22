第二部分

（注：路径为val textFile = sc.textFile("file:///usr/local/hadoop/README.txt")

textFile.count()

）

![image-20240428101839918](/大数据处理实验/pic/image-20240428101839918.png)

![image-20240428102136960](/大数据处理实验/pic/image-20240428102136960.png)

![image-20240428103635009](/大数据处理实验/pic/image-20240428103635009.png)



然后cd /usr/local/spark，bin/spark-shell启动spark

![image-20240428103813280](/大数据处理实验/pic/image-20240428103813280.png)

![image-20240428104037300](/大数据处理实验/pic/image-20240428104037300.png)

![image-20240428131915534](/大数据处理实验/pic/image-20240428131915534.png)

结果：

![image-20240428131945377](/大数据处理实验/pic/image-20240428131945377.png)

（注：flatMap）



用scale语言实现wordcount（有问题？）

![image-20240428110923893](/大数据处理实验/pic/image-20240428110923893.png)



第三部分：spark streaming

先开一个终端：nc -l -p 9999

然后在当前终端（/usr/local/spark/mycode/streaming）下输入：/usr/local/spark/bin/spark-submit --class "WordCountStructuredStreaming" ./target/scala-2.12/simple-project_2.12-1.0.jar

![image-20240428150416261](/大数据处理实验/pic/image-20240428150416261.png)

![image-20240428150631387](/大数据处理实验/pic/image-20240428150631387.png)

![image-20240428150736638](/大数据处理实验/pic/image-20240428150736638.png)
