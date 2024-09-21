package utils;

import java.text.DecimalFormat;

public class HadoopParams {

    public static final String SPARATOR = "\t";

    public static final String subSPARATOR = ",";

    public static final String bufferOutputDirName = "buffer";

    public static final String pageRankOutputDirName = "output";

    public static final String transitionMatrixDirName = "tm";

    public static final String resultFileName = "result.csv";

    public static final DecimalFormat decimalFormat = new DecimalFormat("#.0000000");

    public static final String skipSign = "#";

    public static final float initialPageRank = 1f / 281903f;

    public static int hashCode(String str){
        return str.hashCode();
    }

    public static String[] mapLine(String line){
        if(line.startsWith(HadoopParams.skipSign)){
            return new String[]{};
        }
        return line.trim().split(HadoopParams.SPARATOR);
    }
}
