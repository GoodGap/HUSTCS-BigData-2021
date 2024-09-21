import org.apache.spark.sql.functions._
import org.apache.spark.sql.SparkSession

object WordCountStructuredStreaming{
def main(args: Array[String]){
val spark = SparkSession.builder.appName("StructuredNetworkWordCount"). getOrCreate()
import spark.implicits._
val lines = spark.readStream.format("socket").option("host","localhost"). option("port",9999).load()
val words = lines.as[String].flatMap(_.split(" "))
val wordCounts = words.groupBy("value").count()
val query = wordCounts.writeStream.outputMode("complete"). format("console").start()
query.awaitTermination()
}
}
