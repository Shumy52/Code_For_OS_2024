//
// Created by kali on 3/17/24.
//

#include "Problem1.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int get_line(int fd, char *line, int line_no, int max_length, int *line_length)
{

    char buffer[BUFFER_SIZE];
    int bytes_read;
    int line_count = 0;
    int total_bytes_read = 0;
    int i;

    lseek(fd, 0, SEEK_SET);

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                line_count++;
                if (line_count == line_no) {
                    while(buffer[i]!='\n')
                    {
                        line[*(line_length)++]=buffer[i];
                        i++;
                    }
                    return 0; // Found the line
                }
            }
        }
        total_bytes_read += bytes_read;
    }
    //if we got here, we fucked up somewhere
    return -1;

}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <file_name> <line_number> <max_length>\n", argv[0]);
        return 1;
    }

    // Open the file for reading
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    // Get the line number and max length from command line arguments
    int line_no = atoi(argv[2]);
    int max_length = atoi(argv[3]);

    // Allocate memory for the line buffer
    char *line = (char *)malloc(max_length);
    if (!line) {
        perror("Error allocating memory");
        close(fd);
        return 1;
    }

    int line_length;
    int result = get_line(fd, line, line_no, max_length, &line_length);
    if (result == 0) {
        printf("Line %d: %s\n", line_no, line);
    } else {
        printf("Error reading line %d\n", line_no);
    }

    // Cleanup
    free(line);
    close(fd);

    return 0;
}