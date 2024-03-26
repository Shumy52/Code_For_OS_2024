#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

typedef struct _OPERATION {
    int no1;
    int no2;
    char operator;
} OPERATION;

//typedef struct__attribute__((Packed)) _OPERATION{}

int write_operation(int fd, OPERATION *op, int offset, int no)
{
    off_t check=lseek(fd, offset, SEEK_SET);
    if(check==-1)
    {
        perror("Error seeking file");
        return -1;
    }
    //learn to write code that is very very secure and tight.
    //it will help you progress in embed engineering
    while(no--)
    {
        write(fd, op, sizeof(OPERATION));
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <file_name> <offset> <no1> <no2>\n", argv[0]);
        return 1;
    }

    // Open the file for writing
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Parse command line arguments
    int offset = atoi(argv[2]);
    int no1 = atoi(argv[3]);
    int no2 = atoi(argv[4]);

    // Create an OPERATION structure with the provided values
    OPERATION op = {no1, no2, '+'}; // You can modify this as needed

    // Write the OPERATION structure to the file
    int result = write_operation(fd, &op, offset, 1);
    if (result != 0) {
        printf("Error writing OPERATION structure to file\n");
        close(fd);
        return 1;
    }

    printf("OPERATION structure written to file successfully\n");

    // Close the file
    close(fd);

    return 0;
}