import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.graphx._

object DFS {
  def main(args: Array[String]): Unit = {
    val conf = new SparkConf().setAppName("DFSExample").setMaster("local[*]")
    val sc = new SparkContext(conf)

    // 读取顶点和边数据
    val vertices = sc.textFile("graphx-wiki-vertices.txt")
      .map { line =>
        val Array(id, name) = line.split("\t")
        (id.toLong, name)
      }
    val edges = sc.textFile("graphx-wiki-edges.txt")
      .map { line =>
        val Array(src, dst) = line.split("\t")
        Edge(src.toLong, dst.toLong, 1)
      }

    // 构建Graph对象
    val graph = Graph(vertices, edges)

    // 定义DFS函数
    def dfs(start: VertexId, visited: Set[VertexId]): List[VertexId] = {
      if (visited.contains(start)) {
        List.empty[VertexId]
      } else {
        val newVisited = visited + start
        val neighbors = graph.edges.filter(_.srcId == start).map(_.dstId).collect()
        start :: neighbors.flatMap(neighbor => dfs(neighbor, newVisited)).toList
      }
    }

    // 初始顶点
    val startVertex = 6598434222544540151L

    // 执行DFS
    val visitedOrder = dfs(startVertex, Set.empty[VertexId])

    // 输出访问顺序
    println("DFS访问顺序:")
    visitedOrder.foreach(println)

    sc.stop()
  }
}
