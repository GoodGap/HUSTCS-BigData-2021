package format;

import utils.HadoopParams;

import java.io.*;
import java.text.DecimalFormat;
import java.util.*;

public class ProduceTransitionMatrix {
    private static Map<String, Integer> hashMap;
    private static Map<Integer, List<Integer>> directionMap;
    private static Set<Integer> hashSet;
    private static int nextCode;

    public static void main(String[] args) throws Exception{
        produceMatrixFromString(args);
    }

    public static void produceMatrixFromString(String[] args) throws Exception{
        String inputFile = args[0];
        String outputFilePath = args[1];
        String transitionMatrixPath = args[2];
        String mapUseType = args[3].trim().toLowerCase();

        BufferedReader inputBR = new BufferedReader(new FileReader(inputFile));
        hashMap = new HashMap<String, Integer>();
        directionMap = new HashMap<Integer, List<Integer>>();

        nextCode = 0;
        String line = inputBR.readLine();
        while(line != null){
            if(mapUseType.equals("string")){
                mapString(line);
            }else if(mapUseType.equals("other")){
                mapLines(line);
            }else{
                throw new IllegalArgumentException("Input right mapUseType: String/Other");
            }
            line = inputBR.readLine();
        }
        writeToFile(
                hashMap.values().iterator(),
                hashMap.size(),
                outputFilePath,
                directionMap,
                transitionMatrixPath
        );
    }

    public static void produceMatrixFromInteger(String[] args) throws Exception{
        String inputFile = args[0];
        String outputFilePath = args[1];
        String transitionMatrixPath = args[2];
        String mapUseType = args[3].trim().toLowerCase();

        BufferedReader inputBR = new BufferedReader(new FileReader(inputFile));

        hashSet = new HashSet<Integer>();
        directionMap = new HashMap<Integer, List<Integer>>();

        String line = inputBR.readLine();
        while(line != null){
            if(mapUseType.equals("integer")){
                mapInteger(line);
            }else if(mapUseType.equals("other")){
                mapLines(line);
            }else{
                throw new IllegalArgumentException("Input right mapUseType: Integer/Other");
            }
            line = inputBR.readLine();
        }

        writeToFile(
                hashSet.iterator(),
                hashSet.size(),
                outputFilePath,
                directionMap,
                transitionMatrixPath);
    }

    private static void writeToFile(Iterator<Integer> iter,int iterSize, String outputFilePath,
                                    Map<Integer, List<Integer>> directionMap, String transitionMatrixPath) throws IOException{

        if(!new File(outputFilePath).exists()){
            new File(outputFilePath).mkdir();
        }
        FileWriter pageRank = new FileWriter(outputFilePath + "/pr.txt");

        if(!new File(transitionMatrixPath).exists()){
            new File(transitionMatrixPath).mkdir();
        }
        FileWriter transitionMatrix = new FileWriter(transitionMatrixPath + "/transition.txt");

        // Write pagerank to file
        while(iter.hasNext()){
            int val = iter.next();
            pageRank.write(val + HadoopParams.SPARATOR
                    + HadoopParams.decimalFormat.format(HadoopParams.initialPageRank) + "\n");
        }
        pageRank.close();

        // Write transitionMatrix to file
        for(int key:directionMap.keySet()){
            List<Integer> values = directionMap.get(key);
            for(int val:values){
                transitionMatrix.write(key + HadoopParams.SPARATOR + val + "\n");
            }
        }
        transitionMatrix.close();
    }

    protected static void mapString(String line){
        if(line.trim().startsWith(HadoopParams.skipSign)
                || line.trim().length()<=1
                || line.trim().split(HadoopParams.SPARATOR)[1].equals("")) {return;}

        String[] fromTo = line.trim().split(HadoopParams.SPARATOR);

        // Encode String
        int[] codes = new int[2];
        for(int i=0;i<2;i++){
            if(hashMap.containsKey(fromTo[i])){
                codes[i] = hashMap.get(fromTo[i]);
            }else{
                hashMap.put(fromTo[i], nextCode++);
                codes[i] = hashMap.get(fromTo[i]);
            }
        }

        // Add links
        if(directionMap.containsKey(codes[0])){
            directionMap.get(codes[0]).add(codes[1]);
        }else{
            List<Integer> targetList = new ArrayList<Integer>();
            targetList.add(codes[1]);
            directionMap.put(codes[0], targetList);
        }
    }

    protected static void mapInteger(String line){
        if(line.trim().startsWith("#")
                || line.trim().length()<=1
                ||line.trim().split(HadoopParams.SPARATOR)[1].equals("")){return;}
        String[] fromTo = line.trim().split(HadoopParams.SPARATOR);

        // Add Node
        int[] codes = new int[2];
        for(int i=0;i<2;i++){
            int key = Integer.parseInt(fromTo[i]);
            if(!hashSet.contains(key)){
                hashSet.add(key);
            }
        }
        // Add links
        if(directionMap.containsKey(Integer.parseInt(fromTo[0]))){
            directionMap.get(Integer.parseInt(fromTo[0])).add(codes[1]);
        }else{
            List<Integer> targetList = new ArrayList<Integer>();
            targetList.add(Integer.parseInt(fromTo[1]));
            directionMap.put(Integer.parseInt(fromTo[0]), targetList);
        }
    }

    protected static void mapLines(String line){
        //process each line
        // hashMap should be node -> integer code of node
        // directionMap should be fromNode -> List of toNodes (in integer)
        // Here is codes to add new links to directionMap, and new nodes to hashMap
    }

}
