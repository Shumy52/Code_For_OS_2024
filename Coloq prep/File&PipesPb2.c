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

int main(int argc, char* argv[])
{
    if(argc!=3){
        printf("no param");
        exit(0);
    }

    struct stat statsFirst;
    struct stat statsSecond;

    if(stat(argv[1], &statsFirst)==-1){
        printf("Stats fucked up");
        exit(0);
    }
    if(stat(argv[2], &statsSecond)==-1){
        printf("Stats fucked up");
        exit(0);
    }

    if(! (!S_ISDIR(statsFirst.st_mode) && S_ISDIR(statsSecond.st_mode))){
        printf("No good param");
        exit(0);
    }

    pid_t pids[4];
    int pipeFromP1toP2[2];
    int pipeFromP1toP3[2];

    if(pipe(pipeFromP1toP2)==-1){
        printf("Pipe creation fuckup");
        exit(0);
    }
    if(pipe(pipeFromP1toP3)==-1){
        printf("Pipe creation fuckup");
        exit(0);
    }
    

    pids[2]=fork();
    if(pids[2]!=0){
        pids[3]=fork();
        //forking again, easier to read later
    }
    if(pids[2]==0){ //PROCESS 2 CODE
        char buffer2[255];

        read(pipeFromP1toP2[0], buffer2, sizeof(buffer2));
        //Add char 2?
        
        char path[255];
        sprintf(path, "%s/%s", argv[1], "output_p2.txt");
        int status=open(path, O_CREAT | O_WRONLY);
    }
    else if(pids[3]==0){ //PROCESS 3 CODE
        char buffer3[255];

        read(pipeFromP1toP2[0], buffer3, sizeof(buffer3));
        //Add char 3?

        char path[255];
        sprintf(path, "%s/%s", argv[1], "output_p3.txt");
        int status=open(path, O_CREAT | O_WRONLY);
    }
    else{//PARENT CODE (P1)
        int fileToUse=open(argv[1], O_RDONLY);

        char buffer[255];
        int charsRead=read(fileToUse, buffer, sizeof(buffer));
        if(charsRead<0){
            printf("read failure");
            exit(0);
        }

        write(pipeFromP1toP2[1], buffer, sizeof(buffer));
        write(pipeFromP1toP3[1], buffer, sizeof(buffer));
    }
    waitpid(pids[2], NULL, 0);
    waitpid(pids[3], NULL, 0);

    struct stats stat1;
}