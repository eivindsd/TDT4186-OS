#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//global variable for pipeline bandwidth, i.e. the number of bytes received in the previous second
long bytes_recieved = 0;
//global variable for the cumulative number of bytes received over the pipe so far
long received = 0;

//prints the bandwidth (bytes received) every second, reset the variable for bytes received
//and fires another alarm
static void signal_handler(int signal_sum) {
    printf("Bandwidth %lu\n", bytes_recieved);
    bytes_recieved = 0;
    alarm(1);
}

//handles the SIGUSR1-signal (task c)
static void USR1_handler(int signal_num) {
    printf("Received bytes: %lu\n", received);
}

int main(int argc, char **argv) { 
    //The array pipefd is used to return two file descriptors referring to the ends of the pipe.
    //pipefd[0] refers to the read end of the pipe.  
    //pipefd[1] refers to the write end of the pipe.
    int pipefd[2];
    pid_t pid;

    //error check on signal SIGALRM
    if(signal(SIGALRM, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    //error check on signal SIGUSR1
    if(signal(SIGUSR1, USR1_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    //only accept 2 command line parameters
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <block size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //allocate datablock of given size to be pipelined
    int *buf = malloc(atoi(argv[1]));
    //if malloc() returns NULL, the request has failed
    if(buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    //if error in pipe
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //interestingly the alarm() function is always successful, and no return value
    //is reserved to indicate an error. 
    alarm(1);
    pid = fork();
    int show_pid = 1;

    //If error in fork
    if(pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    //number of bytes to be read/written
    int size = atoi(argv[1]);
    
    while(1) { 
        //in child process
        if(pid == 0) {
            //errorcheck handling on write
            if(write(pipefd[1], buf, size) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }    
        }
        //in the parent process
        else {
            //task c - we need the pid to trigger the SIGUSR1 signal
            if (show_pid) {
                printf("pid %d\n", getpid());
                show_pid = 0;
            }
            int returned;
            returned = read(pipefd[0], buf, size);
            //if error in read
            if(returned == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            received += returned;
            bytes_recieved += returned;
            //task a - output the cumulative number of received bytes after each 
            //read call of the parent process
            //this is commented out below
            printf("Bytes recieved %lu\n", received);
        }
    }
}