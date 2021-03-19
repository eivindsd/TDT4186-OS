#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv) {
    FILE* f;
    if(argc > 1) {
            f = fopen(argv[1], "r");
        }
    while(69){
        char terminal_line[100];
        static char * tokens[100];
        char* input;
        char* output;
        int crocodile=1;
        int m = 0;
        static char * filelines[300];
        int n = 0;
        
        if(argc > 1) { 
            int i = 0;
            while(fgets(terminal_line, sizeof(terminal_line), f)){
                char* line = strdup(terminal_line);
                filelines[i] = line;
                printf("%s\n", filelines[i]);
                i++;
                }
        }
        else {
            printf("wish£ ");
            fgets(terminal_line, sizeof(terminal_line), stdin);
            printf("%s\n", terminal_line);
            terminal_line[strlen(terminal_line)-1] = '\0';
        }
        if (argc > 1) {
            printf("%d", n);
            char* token = strtok(filelines[n], " ");
            while(token != NULL){
                tokens[m] = token;
                token = strtok(NULL, " ");
                m++;
            }
        }
        else {
            char* token = strtok(terminal_line, " ");
            while(token != NULL){
            tokens[m] = token;
            token = strtok(NULL, " ");
            m++;
        } 
        }
        
        printf("Command name: %s\n", tokens[0]);
        if (m > 1) {
            printf("Command parameters: ");
            int n = 1;
            int i = 1;
            while ((strcmp(tokens[n], "<")!=0) && (strcmp(tokens[n], ">")!=0)) {
                printf("%s", tokens[n]);
                n++;
                i = n;
                crocodile = n;
                if (n >= m) {
                    break;
                }
            }  
        printf("\n");
        printf("Redirections: ");
        for (int c = i; c<m; c++) {
            printf(" %s", tokens[c]);
            if(strcmp(tokens[c], ">")==0) {
                output = tokens[c+1];
            }
            if(strcmp(tokens[c], "<")==0) {
                input = tokens[c+1];
            }
        }
        printf("\n");
        }
        tokens[m] = NULL;

        if(strcmp(tokens[0], "cd")==0) {
            if(chdir(tokens[1]) == -1) {
                perror("Error");
            }
            continue;
        }
        else if(strcmp(tokens[0], "exit")==0) {
            exit(0);
        }
        int pid = fork();
        pid_t wpid;
        int status;

        if (pid == 0 ) {
            if(output) {
                int fd_out = creat(output, 0644);
                dup2(fd_out, STDOUT_FILENO);
                tokens[crocodile] = NULL;
            }
            if(input) {
                int fd_in = open(input, O_RDONLY);
                dup2(fd_in, STDIN_FILENO);
                tokens[crocodile] = NULL;
            }
            
            if (execvp(tokens[0], tokens) == -1) {
                perror("Error"); 
            } 
        } 
        else if(pid > 0) {
            do {
                printf("Parent process\n");
            }
            while (wpid == wait(&status) > 0);
        }
        else {
            perror("Failed creating child process");
        } 
        // if (filelines[n+1] == NULL) {
        //     exit(0);
        // }
    
        n++;

}
    
}

