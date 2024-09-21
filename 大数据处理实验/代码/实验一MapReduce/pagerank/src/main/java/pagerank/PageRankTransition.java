package pagerank;

import utils.HadoopParams;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.chain.ChainMapper;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class PageRankTransition {
    public static class TransitionMatrixMapper extends Mapper<Object, Text, IntWritable, IntFloatWritable>{
        @Override
        public void map(Object key, Text value, Context context) throws IOException,InterruptedException{
            String[] fromTo = value.toString().trim().split(HadoopParams.SPARATOR);
            if(value.toString().trim().startsWith(HadoopParams.skipSign)
                    ||fromTo.length<2
                    || fromTo[1].trim().equals("")) {return;}

            String source = fromTo[0];
            String target = fromTo[1];
            context.write(
                    new IntWritable(Integer.parseInt(source)),
                    new IntFloatWritable(Integer.parseInt(target),false)
            );
        }
    }

    public static class PageRankStateMapper extends Mapper<Object, Text, IntWritable, IntFloatWritable>{
        @Override
        public void map(Object key, Text value, Context context) throws IOException, InterruptedException{
            String[] sourceState = value.toString().trim().split(HadoopParams.SPARATOR);
            context.write(
                    new IntWritable(Integer.parseInt(sourceState[0])),
                    new IntFloatWritable(Float.floatToIntBits(Float.parseFloat(sourceState[1])), true)
            );
        }
    }

    public static class MultiplicationReducer extends Reducer<IntWritable, IntFloatWritable, IntWritable, FloatWritable>{
        @Override
        public void reduce(IntWritable key, Iterable<IntFloatWritable> values, Context context) throws IOException, InterruptedException{
            List<IntWritable> components = new ArrayList<IntWritable>();
            float state = 0f;
            for(IntFloatWritable v:values){
                if(v.getState())
                    state = Float.intBitsToFloat(v.getVal().get());
                else
                    components.add(v.getVal());
            }

            float pageNum = components.size();
            for(IntWritable value:components){
                float score = state/pageNum;
                context.write(value, new FloatWritable(score));
            }
        }
    }

    public static void main(String[] args) throws Exception{
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf);
        job.setJarByClass(PageRankTransition.class);

        ChainMapper.addMapper(
                job, TransitionMatrixMapper.class,
                Object.class, Text.class, IntWritable.class, IntFloatWritable.class,
                conf);
        ChainMapper.addMapper(
                job, PageRankStateMapper.class,
                IntWritable.class, IntFloatWritable.class, IntWritable.class, IntFloatWritable.class,
                conf);

        job.setReducerClass(MultiplicationReducer.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(FloatWritable.class);

        MultipleInputs.addInputPath(job, new Path(args[0]), TextInputFormat.class, TransitionMatrixMapper.class);
        MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, PageRankStateMapper.class);
        FileOutputFormat.setOutputPath(job, new Path(args[2]));

        job.waitForCompletion(true);
    }
}

class IntFloatWritable implements Writable{
    private IntWritable val;
    private BooleanWritable state;

    public IntFloatWritable(int v, boolean s){
        val = new IntWritable(v);
        state = new BooleanWritable(s);
    }
    public IntFloatWritable(){
        val = new IntWritable();
        state = new BooleanWritable();
    }

    public boolean getState(){
        return state.get();
    }

    public IntWritable getVal(){
        return val;
    }

    public void readFields(DataInput dataInput) throws IOException{
        val.readFields(dataInput);
        state.readFields(dataInput);
    }

    public void write(DataOutput dataOutput) throws IOException{
        val.write(dataOutput);
        state.write(dataOutput);
    }
}
