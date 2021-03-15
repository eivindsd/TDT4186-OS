#include <stdio.h>
#include <string.h>


void tokenize() {
    char terminal_line[30];
    char * tokens[20];
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
            printf("%d", i);
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
    
}


int main() {
    while(69){
        tokenize();
        }
}
