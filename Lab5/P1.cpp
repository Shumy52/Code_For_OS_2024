//
// Created by kali on 3/26/24.
//
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

void search_dir(char *dir_name, char* searched_name){
    struct dirent *dp;
    DIR *dir=opendir(dir_name);
    if(!dir) {
        printf("ERROR \n invalid directory path");
        exit(-1);
    }

    char buildPath[400];
    while((dp=readdir(dir))!=nullptr) {
        if (strcmp(dp->d_name, searched_name) == 0) {
            sprintf(buildPath, "%s/%s", dir_name, dp->d_name);
            printf("Found file! Path is:%s", buildPath);
        }
    }
}

void problem1(char *path, char *name) {

    if (path == nullptr || name == nullptr) {
        printf("ERROR\nInvalid path or filename");
    }


    //struct dirent *dp; //This is used only for iterating over directories. Remember, this is a structure of directories! A link of them!
//    DIR *dir = opendir(path);
//    if (!dir) {
//        printf("ERROR \n invalid directory path");
//        exit(-1);
//    }
//
//    //dp=readdir(dir); //paired with the struct above... doesn't make too much sense here
//    if(dp==nullptr){
//        printf("Error on readdir!");
//        exit(-1);
//    }


    struct stat fileStat;
    char buildPath[400];
    sprintf(buildPath, "%s/%s", path, name);

    if (stat(buildPath, &fileStat) == -1) {
        printf("Error getting stats, most likely path invalid");
        exit(-1);
    }

    if (!S_ISDIR(fileStat.st_mode)) {
        printf("File path does not lead to a directory");
        exit(-1);
    }

    printf("Path:%s", buildPath);


}

int main(int argc, char*argv[])
{
        char *path=nullptr;
        char *name=nullptr;

        if(argc==1)
            printf("USAGE: P1.cpp dir/file <path=/yourPath> <name=yourName> "),exit(0);


        for(int i=1; i<argc; i++)
        {
            if(strncmp(argv[i], "path=", 5)==0){
                path=argv[i]+5;
            }
            if(strncmp(argv[i], "name=", 5)==0){
                name=argv[i]+5;
            }
        }

    if(strncmp(argv[1], "dir", 3)){
        problem1(path, name);
    }
    else search_dir(path, name);

}


