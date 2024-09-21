package utils;
import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.io.File;

public class FuncTools {
    public static void transToCSV(String[] args) throws IOException{
        BufferedReader pr = new BufferedReader(new FileReader(args[1] + "/part-r-00000"));
        FileWriter fileWriter = new FileWriter(args[2]);

        // PageRank Scores
        Map<String, String> page_pr = new HashMap<String, String>();
        String line = pr.readLine();
        while (line != null) {
            page_pr.put(
                    line.split(HadoopParams.SPARATOR)[0],
                    line.split(HadoopParams.SPARATOR)[1]);
            line = pr.readLine();
        }
        pr.close();

        fileWriter.write("source,target,value\n");

        for(File file: new File(args[0]).listFiles()){
            if(file.toString().contains(".DS_Store")) continue;
            BufferedReader transition = new BufferedReader(new FileReader(file));
            line = transition.readLine();

            while (line != null) {
                String[] from_tos = line.split(HadoopParams.SPARATOR);
                if(line.startsWith(HadoopParams.skipSign)
                        ||from_tos.length<2
                        ||from_tos[1].equals("")){
                    line = transition.readLine();
                    continue;
                }
                String value = page_pr.get(from_tos[1]);
                fileWriter.write(from_tos[0]
                        + HadoopParams.subSPARATOR
                        + from_tos[1]
                        + HadoopParams.subSPARATOR
                        + value + "\n");
                line = transition.readLine();
            }
            transition.close();
        }
        fileWriter.close();
    }

    public static void deleteFiles(File file){
        if(!file.exists()) return;

        if(file.isFile() || file.list()==null){
            file.delete();
            return;
        }else{
            File[] files = file.listFiles();
            for(File f:files){
                deleteFiles(f);
            }
            file.delete();
            return;
        }
    }
}
