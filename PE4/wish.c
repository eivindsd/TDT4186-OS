#include <stdio.h>
#include <string.h>


// void tokenize(char input) {
//     char *token = strtok(input, " ");
//     while(token != NULL) {
//         printf("%s\n", token);
//         printf("Kuk");
//         token = strtok(NULL, " ");
//     }
    
// }


int main() {
while(1) {
    char terminal_line[30];
    char * tokens[20];
    int m = 0;
    printf("wish£ ");
    fgets(terminal_line, sizeof(terminal_line), stdin);
    char* token = strtok(terminal_line, " ");
    while(token != NULL){
        tokens[m] = token;
        token = strtok(NULL, " ");
        printf("%lu", sizeof(tokens));
        m++;
    }
    printf("%lu", sizeof(tokens));
    printf("Command name: %s\n", tokens[0]);
    printf("Command parameters");
    int n = 1;
    int i;
    while ((strcmp(tokens[n], "<")!=0) && (strcmp(tokens[n], ">")!=0)) {
        printf(" %s", tokens[n]);
        n++;
        i = n;
    }
    printf("\n");
    printf("Redirections: ");
    for (int i; n<sizeof(tokens);n++) {
        printf(" %s", tokens[n]);
    }
    }

}

