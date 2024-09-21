import java.io.File;

import pagerank.PageRankSum;
import pagerank.PageRankTransition;
import pagerank.ProduceInitPageRankOnHDFS;
import utils.FuncTools;
import utils.HadoopParams;

public class Driver {
    public static void main(String [] args) throws Exception{
        DriverInitializer initializer = new DriverInitializer(args);

        String[] args0 = {
                initializer.inputFile,
                initializer.initPageRankPath,
                initializer.inputFile
        };
        ProduceInitPageRankOnHDFS.main(args0);

        PageRankTransition pageRankTransition = new PageRankTransition();
        PageRankSum pageRankSum = new PageRankSum();
        for(int i=1;  i<=initializer.convergence;  i++) {
            initializer.update(i);
            String[] args1 = {
                    initializer.inputFile,
                    initializer.prevPageRankPath,
                    initializer.middleStatePath
            };
            pageRankTransition.main(args1);

            String[] args2;
            if(i==initializer.convergence){
                args2 = new String[]{
                        initializer.middleStatePath,
                        initializer.prevPageRankPath,
                        initializer.nextPageRankPath,
                        initializer.beta
                };
            }else{
                args2 =  new String[]{
                        initializer.middleStatePath,
                        initializer.initPageRankPath,
                        initializer.nextPageRankPath,
                        initializer.beta
                };
            }
            pageRankSum.main(args2);
        }
    }
}

class DriverInitializer{
    public String inputFile;
    public String transitionMatrixPath;
    public String initPageRankPath;
    public String finalPageRankPath;
    public String finalResultsFile;
    public int convergence;
    public String beta;
    public String middleResultsPath;
    public String prevPageRankPath;
    public String middleStatePath;
    public String nextPageRankPath;

    public DriverInitializer(String[] args){
        inputFile = args[0];
        middleResultsPath = args[1];
        finalResultsFile = args[2] +"/" +HadoopParams.resultFileName;
        convergence = Integer.parseInt(args[3]);
        beta = args[4].trim();

        transitionMatrixPath = middleResultsPath + "/" + HadoopParams.transitionMatrixDirName;
        initPageRankPath = middleResultsPath + "/"+ HadoopParams.pageRankOutputDirName+0;
        finalPageRankPath = middleResultsPath + "/"+ HadoopParams.pageRankOutputDirName+convergence;
    }

    private void emptyFiles(){
        FuncTools.deleteFiles(new File(middleResultsPath));
        FuncTools.deleteFiles(new File(finalResultsFile));
        new File(middleResultsPath).mkdir();
    }

    public void update(int i){
        prevPageRankPath= middleResultsPath + "/"+ HadoopParams.pageRankOutputDirName+(i-1);
        middleStatePath = middleResultsPath + "/" + HadoopParams.bufferOutputDirName+i;
        nextPageRankPath = middleResultsPath + "/" + HadoopParams.pageRankOutputDirName+i;
    }
}
