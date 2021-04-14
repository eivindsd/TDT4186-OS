#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int main(int argc, char **argv) { 
    
    pid_t pid;
    //path to FIFO special file
    char *myfifo = "/tmp/myfifo";
    int fd_write = -1;
    int fd_read = -1;

    //error check on signal SIGALRM
    if(signal(SIGALRM, signal_handler) == SIG_ERR) {
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
    
    //errorcheck on unlink
    if(unlink(myfifo) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    //errorcheck on mkfifo
    if ((mkfifo(myfifo, 0666)) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    alarm(1);
    pid = fork();

    //If error in fork
    if(pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    int size = atoi(argv[1]);
    while(1) { 
        //in child process
        if(pid == 0) {  
            if (fd_write == -1) {
                //open write-end
                fd_write = open(myfifo, O_WRONLY);
            }
            //errorhandling on write
            if(write(fd_write, buf, size) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }    
        }
        //in the parent process
        else {
            if (fd_read == -1) {
                //open read-end
                fd_read = open(myfifo, O_RDONLY);
            }
            int returned;
            //read from pipe
            returned = read(fd_read, buf, size);
            //if error in read
            if(returned == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            received += returned;
            bytes_recieved += returned;
        } 
    }
}