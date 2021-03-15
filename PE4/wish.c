#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


char** tokenize() {
    char terminal_line[30];
    static char * tokens[20];
    int m = 0;
    printf("wish£ ");
    fgets(terminal_line, sizeof(terminal_line), stdin);
    char* token = strtok(terminal_line, " ");
    while(token != NULL){
        tokens[m] = token;
        token = strtok(NULL, " ");
        m++;
    }
    printf("Command name: %s\n", tokens[0]);
    if (m > 1) {
        printf("Command parameters: ");
        int n = 1;
        int i = 1;
        while ((strcmp(tokens[n], "<")!=0) && (strcmp(tokens[n], ">")!=0)) {
            printf(" %s", tokens[n]);
            n++;
            i = n;
            if (n >= m) {
                break;
            }
        }
    printf("\n");
    printf("Redirections: ");
    for (int c = i; c<m; c++) {
        printf(" %s", tokens[c]);
    }
     printf("\n");
    }
    return tokens;
}

void execute() {

}

int main() {
    while(69){
        char** tokens = tokenize();
        int forkstatus = fork();
        pid_t wpid;
        int status;
      
        if (forkstatus == 0 ) {
            if (execv(tokens[0], tokens) == -1) {
                perror("Det ble kuk"); 
            }
            exit(EXIT_FAILURE); 
        } 
        else {
            printf("Parent process\n");
            while (wpid == wait(&status) > 0);
        }      
}
}

