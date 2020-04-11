#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include<unistd.h>
#include<vector>


using namespace std;
int continuousInputProcessing(pid_t p1 ,pid_t p2 ,pid_t p3 ,pid_t p4,pid_t dPid)
{
    while(!std::cin.eof()){
        std::string inputOutput;
        std::getline(std::cin, inputOutput);
        
        if(inputOutput.size() > 0) {
            std::cout<<inputOutput<<std::endl;
            std::cout.flush();
        }
    }
    int pipeStatus1,pipeStatus2,pipeStatus3,pipeStatus4;
//    kill(p1, SIGINT);
//    waitpid(p1, &pipeStatus1, 0);
//    kill(p2,SIGINT);
//    waitpid(p2, &pipeStatus2, 0);
//    kill(p3,SIGINT);
//    waitpid(p3, &pipeStatus3, 0);
//    kill(p4,SIGINT);
//    waitpid(p4, &pipeStatus4, 0);
//    kill(dPid,SIGTERM);
//    waitpid(dPid, &pipestatus5, 0);
    
    return 0;
}

void showError(string message) {
    cerr << "Error: "<<message<<endl;
}

int main(int argc,char **argv) {
    int pipeStatus;
    int Pipe2[2];
    int Pipe1[2];
    
    
    unsigned int maxNumberOfStreets = 10, maxNumberOfLines = 5,maxWaitTime = 5 ,maxRandomNumberRange = 20;
    std::string sting_s, sting_n, sting_l, sting_c;
    int argument;
//    pid_t driverPid = getpid();
    opterr = 0;
    while ((argument = getopt(argc,argv, "s:n:l:c:")) != -1) {
        switch (argument) {
                
            case 's':
                sting_s = optarg;
                maxNumberOfStreets = std::atoi(sting_s.c_str());
                if (maxNumberOfStreets < 2){
                    showError("Number of streets should be greater than 1");
                    exit(1);
                }
                break;
                
            case 'n':
                sting_n = optarg;
                maxNumberOfLines = std::atoi(sting_n.c_str());
                if (maxNumberOfLines < 1){
                    showError("Number of lines in each street should be atleast 1");
                    exit(1);
                }
                break;
                
            case 'l':
                sting_l = optarg;
                maxWaitTime = std::atoi(sting_l.c_str());
                if (maxWaitTime < 5){
                    showError("Wait time should be atleast 5 seconds");
                    exit(1);
                }
                break;
                
            case 'c':
                sting_c = optarg;
                maxRandomNumberRange = std::atoi(sting_c.c_str());
                if (maxRandomNumberRange < 1){
                    showError("Range of coordi=nates should be grater than or equal to |1|");
                    exit(1);
                }
                break;
                
            case '?':
                showError("invalid switch :" + to_string(optopt));
                exit(1);
                break;
        }
    }
    
    pipe(Pipe1);
    pipe(Pipe2);
    
    pid_t pIdStreetGenerator = fork();
    if(pIdStreetGenerator == 0){
        dup2(Pipe1[1],STDOUT_FILENO);
        close(Pipe1[1]);
        close(Pipe1[0]);
        execv("./rgen",argv);
    }
    else{
        pid_t pIdGraphGenerator = fork();
        if( pIdGraphGenerator != 0){
            pid_t pIdRouteFinder = fork();
            if (pIdRouteFinder == 0){
                dup2(Pipe2[0],STDIN_FILENO);
                close(Pipe2[1]);
                close(Pipe2[0]);
                execv("./a2ece650", argv);
            }
            int input;
            pid_t pIdterminator = fork();
            if (pIdterminator == 0) {
                dup2(Pipe2[1],STDOUT_FILENO);
                close(Pipe2[1]);
                close(Pipe2[0]);
            input=continuousInputProcessing(pIdGraphGenerator,pIdStreetGenerator,pIdRouteFinder,pIdterminator,pIdterminator);
            }
            pid_t n = wait(NULL);
            kill(pIdStreetGenerator, SIGTERM);
            waitpid(pIdStreetGenerator, &pipeStatus, 0);
            kill(pIdGraphGenerator,SIGTERM);
            waitpid(pIdGraphGenerator, &pipeStatus, 0);
            kill(pIdRouteFinder,SIGTERM);
            waitpid(pIdRouteFinder, &pipeStatus, 0);
            
            
            
            return 0;
        }
        else{
            char *argv1[3];
            argv1[1] = (char *)"a1ece650.py";
            argv1[2] = nullptr;
            argv1[0] = (char *)"python";
            dup2(Pipe1[0],STDIN_FILENO);
            close(Pipe1[1]);
            close(Pipe1[0]);
            dup2(Pipe2[1], STDOUT_FILENO);
            close(Pipe2[1]);
            close(Pipe2[0]);
            execvp("python", argv1);
        }
    }
    return 0;
}



