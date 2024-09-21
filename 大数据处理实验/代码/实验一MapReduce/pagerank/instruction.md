cd /usr/local/hadoop
./sbin/start-dfs.sh

./bin/hdfs dfs -rm -r user/hadoop/output

$ cd pagerank-new
$ hdfs dfs -put ./input /

$ hadoop jar out/artifacts/mapred_pagerank/mapred-pagerank.jar Driver /input/web-Stanford.txt /user/hadoop/output/middle_results /output 40 0.15

$ hdfs dfs -get /output/middle_results/output5/part-r-00000 ./output/pagerank


mapper映射输入数据集，输出以下两类数据：
1、当前网页拥有超链接的网页列表，以及当前网页的pr值；
2、当前网站对超链接网页列表各网页的投票，以及投票的权重。

reducer已网页为key，统计每个网页收到的投票值总和，然后计算pr值，最后输出本轮计算的结果

