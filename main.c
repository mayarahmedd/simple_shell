#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>

char directory[100];


void handler(int sig) {

    waitpid(-1, NULL, WNOHANG);


    FILE *fs;

    char *filename = (char *) malloc(1 + strlen(directory) + 12);
    strcpy(filename, directory);
    strcat(filename, "/logfile.txt");

    fs = fopen(filename, "a");
    if (fs == NULL) {
        printf("failed to open and create file.\n");


    } else {
        fprintf(fs, "Child Process was Terminated \n");

    }

    fclose(fs);


}


int main() {


    getcwd(directory, 100);

    signal(SIGCHLD, handler);


    while (1) {


        char commands[1000];
        char *parsed;
        char *argv[200];
        int index = 0;
        int hasAmpersand = 0;

        printf("my shell <<");
        fgets(commands, 1000, stdin);
        strtok(commands, "\n");

        parsed = strtok(commands, " ");

        while (parsed != NULL) {

            if (strcmp(parsed, "&") == 0) {
                hasAmpersand = 1;
                break;
            } else {
                argv[index] = (char *) malloc(strlen(parsed));


                strcpy(argv[index], parsed);

                parsed = strtok(NULL, " ");


                index++;

            }
        }
        argv[index] = NULL;


        if (strcmp(argv[0], "exit") == 0) {
            break;
        }


        if (strcmp(argv[0], "cd") == 0) {
            if (argv[1] != NULL) {


                chdir(argv[1]);

            } else {

                chdir("..");
                printf("%s", directory);
            }
        } else {


            pid_t pid = fork();
            int status;

            if (pid < 0) {
                printf("child process fork failed");
            } else if (pid == 0) {
                execvp(argv[0], argv);


            } else {
                if (hasAmpersand == 0) {

                    waitpid(pid, &status, 0);

                }
            }
        }
    }
    return 0;

}
