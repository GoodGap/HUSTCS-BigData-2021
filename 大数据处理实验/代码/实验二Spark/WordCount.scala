import org.apache.spark.rdd.RDD
import org.apache.spark.{SparkConf, SparkContext}

/** * Scala原生实现wordcount */
object WordCount {
  def main(args: Array[String]): Unit = {

    val list = List("cw is cool", "wc is beautiful", "andy is beautiful", "mike is cool")
    /** * 第一步，将list中的元素按照分隔符这里是空格拆分，然后展开 * 先map(_.split(" "))将每一个元素按照空格拆分 * 然后flatten展开 * flatmap即为上面两个步骤的整合 */
    val res0 = list.map(_.split(" ")).flatten
    val res1 = list.flatMap(_.split(" "))
    println("第一步结果")
    println(res0)
    println(res1)

    /** * 第二步是将拆分后得到的每个单词生成一个元组 * k是单词名称，v任意字符即可这里是1 */
    val res3 = res1.map((_, 1))
    println("第二步结果")
    println(res3)
    /** * 第三步是根据相同的key合并 */
    val res4 = res3.groupBy(_._1)
    println("第三步结果")
    println(res4)
    /** * 最后一步是求出groupBy后的每个key对应的value的size大小，即单词出现的个数 */
    val res5 = res4.mapValues(_.size)
    println("最后一步结果")
    println(res5.toBuffer)
  }
}
