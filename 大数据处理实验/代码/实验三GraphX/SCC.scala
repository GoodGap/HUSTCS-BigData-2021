import org.apache.spark.graphx._
import org.apache.spark.{SparkConf, SparkContext}

object SCC {
  def main(args: Array[String]): Unit = {
    val conf = new SparkConf().setAppName("SCC").setMaster("local[*]")
    val sc = new SparkContext(conf)

    // 读取顶点和边的数据
    val vertices = sc.textFile("graphx-wiki-vertices.txt").map { line =>
      val fields = line.split("\t")
      (fields(0).toLong, fields(1))
    }
    val edges = sc.textFile("graphx-wiki-edges.txt").map { line =>
      val fields = line.split("\t")
      Edge(fields(0).toLong, fields(1).toLong, 1)
    }

    // 构建图
    val defaultVertex = ("")
    val graph = Graph(vertices, edges, defaultVertex)

    // 运行强连通分量算法
    val scc = graph.stronglyConnectedComponents(5).vertices

    // 将结果收集并按照强连通分量id分组
    val sccGroups = scc.map(_.swap).groupByKey().values

    // 打印结果
    var sccCount = 0
    sccGroups.collect().foreach { group =>
      sccCount += 1
      val vertices = group.mkString(", ")
      println(s"强联通分量$sccCount 包含顶点 $vertices")
    }

    sc.stop()
  }
}

