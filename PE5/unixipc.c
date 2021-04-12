#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) { 

    //The array pipefd is used to return two file descriptors referring to the ends of the pipe.
    //pipefd[0] refers to the read end of the pipe.  
    //pipefd[1] refers to the write end of the pipe.
    int pipefd[2];
    pid_t pid;
    char buf[atoi(argv[1])];
    int received = 0;
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <block size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //If error in pipe
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();

    //If error in fork
    if(pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    while(1) {
        //in child process
        if(pid == 0) {
            int size = sizeof(argv[1]);
            char a[size];
            printf("Write %s\n", argv[1]);
            close(pipefd[0]);   /* Close unused read end */
            //errorcheck handling on write, and write argv[1] to pipe
            if(write(pipefd[1], argv[1], size) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }    
        }
        //in the parent process
        else {
            close(pipefd[1]);  /* Close unused write end */
            int returned;
            returned = read(pipefd[0], buf, sizeof(buf));
            printf("Returned %d\n", returned);
            if(read(pipefd[0], buf, sizeof(buf)) == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            //cumulative number of received bytes
            received += returned;
            printf("Buffer %s\n", buf);
            printf("Recieved %d\n", received);
        }
    }
}