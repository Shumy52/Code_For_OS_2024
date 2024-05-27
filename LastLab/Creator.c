#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){

    char* nameOfShm = argv[1];
    int size=atoi(argv[2]);

    int shared= shm_open(nameOfShm, O_CREAT | O_RDONLY, 0666);
    //Look at the flags, O_CREAT! This "creates" the shared memory stuff. Basically
    //creates a pointer to some memory that will be shared

    ftruncate(shared, size);
    //Here we set the size we want. So in theory this is all in the big storage
    //Next up, we'll map it to the RAM for easier access

    char *shm_ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);
    //the first parameter should be a multiple of the pagefile size. Don't fuck with it

}