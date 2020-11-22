#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

static int bg = 0;
static int child_terminated = 0;

void init_shell() {
    printf("**************************************************************************\n");
    printf("*                                                                        *\n");
    printf("*                               Omar Moghazy                             *\n");
    printf("*                          5504 Group 5 Section 2                        *\n");
    printf("*                               Simple Shell                             *\n");
    printf("*                                                                        *\n");
    printf("**************************************************************************\n");
    char* username = getenv("USER");
    printf("\n\n\nUser: %s", username);
}

char* read_input() {
    // readline
    char* input = readline("\n\n>:");
    if(strlen(input) > 0) add_history(input);

    // no readline
    /*char* input = (char*)malloc(sizeof(char)*500);
    printf("\n\n>:");
    scanf("%[^\n]", input);
    getchar();*/
    return input;
}

void parse_input(char* input, char** args) {
    char* token = strtok(input, " ");
    int i = 0;
    while(token) {
        args[i] = (char*)malloc(20*sizeof(char));
        strcpy(args[i], token);
        i++;
        token=strtok(NULL, " ");
    }
    args[i] = NULL;
    if(strcmp("&",args[i-1]) == 0) bg = 1;
    else bg = 0;
}

void child_terminate_handler() {
    //printf("\nChild process terminated\n");
    write(STDOUT_FILENO,"Child process was terminated\n", sizeof(char) * 29);
    FILE* f = fopen("log.txt", "a");
    //fprintf(f,"Child process was terminated\n");
    fwrite("Child process was terminated\n", sizeof(char), 29, f);
    fclose(f);
}

void execute_linux_commands(char** args) {
    pid_t pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args)<0)
            printf("Execution failed\n");
        exit(0);
    }

    else if(pid < 0){
        printf("Fork Failed");
        return;
        }
    else {
        if(!bg) wait(NULL);
        return;
    }
}

void execute_command(char**args) {
    if(strcasecmp(args[0],"exit")==0)
        exit(0);
    else
        execute_linux_commands(args);
}

int main()
{
    init_shell();
    signal(SIGCHLD,child_terminate_handler);
    while(1) {
        char* input_line = read_input();
        if(strlen(input_line) == 0) continue;
        char* args[100];
        parse_input(input_line, args);
        execute_command(args);
    }

    return 0;
}
