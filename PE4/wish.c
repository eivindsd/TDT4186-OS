#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


// char** tokenize() {
//     char terminal_line[30];
//     static char * tokens[20];
//     int m = 0;
//     printf("wish£ ");
//     fgets(terminal_line, sizeof(terminal_line), stdin);
//     terminal_line[strlen(terminal_line)-1] = '\0';
//     char* token = strtok(terminal_line, " ");
//     while(token != NULL){
//         tokens[m] = token;
//         token = strtok(NULL, " ");
//         m++;
//     }
//     printf("Command name: %s\n", tokens[0]);
//     if (m > 1) {
//         printf("Command parameters: ");
//         int n = 1;
//         int i = 1;
//         while ((strcmp(tokens[n], "<")!=0) && (strcmp(tokens[n], ">")!=0)) {
//             printf(" %s", tokens[n]);
//             n++;
//             i = n;
//             if (n >= m) {
//                 break;
//             }
//         }
//     printf("\n");
//     printf("Redirections: ");
//     for (int c = i; c<m; c++) {
//         printf(" %s", tokens[c]);
//     }
//      printf("\n");
//     }
//     tokens[m] = NULL;
//     return tokens;
// }


int main() {
    while(69){
        //char** tokens = tokenize();
        char terminal_line[30];
        static char * tokens[20];
        int m = 0;
        printf("wish£ ");
        fgets(terminal_line, sizeof(terminal_line), stdin);
        terminal_line[strlen(terminal_line)-1] = '\0';
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
        tokens[m] = NULL;
        int pid = fork();
        pid_t wpid;
        int status;
      
        if (pid == 0 ) {
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
}
}

