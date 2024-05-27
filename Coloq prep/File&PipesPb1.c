#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

//CONFIRMED IT WORKS!

int main(int argc, char* argv[])
{
    if(argc!=3){
        printf("Use me right, dumbass");
        exit(-1);
    }

    struct stat statsFirst;
    struct stat statsSecond;

    if(stat(argv[1], &statsFirst)==-1){
        printf("Fuckup line 21");
        exit(-10);
    }
    if(stat(argv[2], &statsSecond)==-1){
        printf("Fuckup line 21");
        exit(-10);
    }

    if(!(!S_ISDIR(statsFirst.st_mode) && S_ISDIR(statsSecond.st_mode))){
        printf("Dumbass");
        exit(-11);
    }
    
    char buildPath[255];

    sprintf(buildPath, "%s/%s", argv[2], "results.txt");

    int file=open(buildPath, O_CREAT | O_WRONLY);
    if(file<0){
        printf("File creation fuckup");
        exit(-12);
    }
    //coms pipe
    int pipeParentToChild[2];
    int pipeChildToParent[2];
    int status=pipe(pipeParentToChild);
    if(status==-1){
        printf("Pipe creation fuckup");
        exit(-69);
    }
    status=pipe(pipeChildToParent);


    pid_t pid=fork();
    if(pid==0){ //child code
        int val1;
        char op;
        int val2;
        int result;

        read(pipeParentToChild[0], &val1, sizeof(int));
        read(pipeParentToChild[0], &op, sizeof(char));
        read(pipeParentToChild[0], &val2, sizeof(int));

        if(op=='+'){
            result=val1+val2;
        }
        else{
            result=val1-val2;
        }

        write(pipeChildToParent[1], &result, sizeof(int));

        return 0;
    }
    else{//parent code
        int fileToUse=open(argv[1], O_RDWR);
        char buffer[255];

        status=read(fileToUse, buffer, 255);
        int val1;
        char op;
        int val2;

        sscanf(buffer, "%d%c%d", &val1, &op, &val2);

        write(pipeParentToChild[1], &val1, sizeof(int));
        write(pipeParentToChild[1], &op, sizeof(char));
        write(pipeParentToChild[1], &val2, sizeof(int));

        int result;
        read(pipeChildToParent[0], &result, sizeof(int));

        char resultString[255];
        sprintf(resultString, "%d", result);

        write(fileToUse, &resultString, sizeof(resultString));

    }
    waitpid(pid, NULL, 0);

    return 0;
}