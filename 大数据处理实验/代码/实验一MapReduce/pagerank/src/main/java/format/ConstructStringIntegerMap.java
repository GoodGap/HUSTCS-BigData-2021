package format;

import com.google.protobuf.TextFormat;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import utils.FuncTools;
import utils.HadoopParams;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class ConstructStringIntegerMap {
    public static class ConstructStringIntegerMapper extends Mapper<Object, Text,IntWritable, Text>{
        @Override
        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException{
            String[] strings = HadoopParams.mapLine(value.toString());
            for(String v: strings){
                context.write(
                        new IntWritable(HadoopParams.hashCode(v)>>>16),new Text(v)
                );
            }
        }
    }

    public static class ConstructStringIntegerCombiner extends Reducer<IntWritable, Text, IntWritable, Text>{
        @Override
        public void reduce(IntWritable key, Iterable<Text> values, Context context)
                throws IOException, InterruptedException{
            Set<Text> textSet = new HashSet<Text>();
            for(Text v: values){
                if(!textSet.contains(v)){
                    textSet.add(v);
                    context.write(key,v);
                }
            }
        }
    }

    public static class ConstructStringIntegerPartitioner extends Partitioner<IntWritable, Text>{
        public int getPartition(IntWritable key, Text value, int numOfReduceTasks){
            return key.get()%numOfReduceTasks;
        }
    }

    public static class ConstructStringIntegerReducer extends Reducer<IntWritable, Text, IntWritable,Text>{
        @Override
        public void reduce(IntWritable key, Iterable<Text> values, Context context)
                throws IndexOutOfBoundsException,IOException,InterruptedException{
            int nextCode = 0;
            Map<String, Integer>hashMap = new HashMap<String, Integer>();
            for(Text t:values){
                String s = t.toString();
                if(!hashMap.containsKey(s)){
                    hashMap.put(s, nextCode++);
                    if(nextCode >= 0xFFFF){
                        throw new IndexOutOfBoundsException("The count of Strings is out of bounds");
                    }
                }
            }

            for(String s:hashMap.keySet()){
                int code = (key.get()<<16) | hashMap.get(s);
                context.write(
                        new IntWritable(code), new Text(s)
                );
            }
        }
    }

    public static void main(String[] args) throws Exception{
        String inputPath = args[0];
        String mapPath = args[1];
        FuncTools.deleteFiles(new File(mapPath));

        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf);

        job.setJarByClass(ConstructStringIntegerMap.class);
        job.setMapperClass(ConstructStringIntegerMapper.class);
        job.setCombinerClass(ConstructStringIntegerCombiner.class);
        job.setPartitionerClass(ConstructStringIntegerPartitioner.class);
        job.setReducerClass(ConstructStringIntegerReducer.class);

        job.setInputFormatClass(TextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(Text.class);

        FileInputFormat.addInputPath(job, new Path(inputPath));
        FileOutputFormat.setOutputPath(job, new Path(mapPath));

        job.waitForCompletion(true);
    }
}
