package pagerank;

import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.NullWritable;
import utils.HadoopParams;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

import java.io.IOException;

public class ProduceInitPageRankOnHDFS {
    public static class PageRankVecMapper extends Mapper<Object, Text, IntWritable, NullWritable> {
        @Override
        public void map(Object key, Text value, Context context) throws IOException,InterruptedException{
            String[] fromTo = value.toString().trim().split(HadoopParams.SPARATOR);
            if(value.toString().trim().startsWith(HadoopParams.skipSign) || fromTo.length<2 || fromTo[1].trim().equals("")) return;
            try {
                context.write(new IntWritable(Integer.parseInt(fromTo[0])),NullWritable.get());
                context.write(new IntWritable(Integer.parseInt(fromTo[1])),NullWritable.get());
            }catch (NumberFormatException e){
                return;
            }
        }
    }

    public static class PageRankVecCombiner extends Reducer<IntWritable, NullWritable, IntWritable, NullWritable> {
        @Override
        public void reduce(IntWritable key, Iterable<NullWritable> values, Context context) throws IOException, InterruptedException{
            context.write(key, NullWritable.get());
        }
    }

    public static class PageRankVecReducer extends Reducer<IntWritable, NullWritable, IntWritable, FloatWritable> {
        @Override
        public void reduce(IntWritable key, Iterable<NullWritable> values, Context context) throws IOException, InterruptedException{
            context.write(key, new FloatWritable(HadoopParams.initialPageRank));
        }
    }
    public static void main(String[] args) throws Exception{
        String inputPath = args[0];
        String pageRankVecPath = args[1];

        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf);
        job.setJarByClass(ProduceInitPageRankOnHDFS.class);

        job.setMapperClass(PageRankVecMapper.class);
        job.setCombinerClass(PageRankVecCombiner.class);
        job.setReducerClass(PageRankVecReducer.class);

        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(NullWritable.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(FloatWritable.class);

        job.setInputFormatClass(TextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        FileInputFormat.addInputPath(job, new Path(inputPath));
        FileOutputFormat.setOutputPath(job, new Path(pageRankVecPath));
        job.waitForCompletion(true);
    }
}
