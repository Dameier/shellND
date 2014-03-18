#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

char *cdir;
/*
 *function to change the current working directory.
 *
 *issues: if the user types cd .. it just tacs it on the end.
 *This works but could be done better.
 */
void exec(char *nextData) {
    char *envargs[20];
    int count = 0;
    int size = 20;
    while (nextData != NULL) {
       if (count == size) {
            size = size * 2;
            *envargs = (char *)realloc(*envargs, size);
       }
       envargs[count] = nextData;
       count++;
       nextData = strtok(NULL, " ");
    }
    envargs[count] = (char *)0;
    pid_t pID = fork();
    if(pID == 0) {
        execv(envargs[0], envargs);
        int execReturn = execv(envargs[0], envargs);
        if (execReturn == -1){
            printf("execv has failed.");
        }
        exit(0);
    }else if(pID < 0){
        printf("nFailed to fork.\n");
    }else{
        printf("parent process\n");
    }
}

void cd(char *nextData) {
    DIR *dir;
    char *string = malloc(1024);
    nextData = strtok(NULL, " ");
    if (nextData == NULL) {
        cdir = getenv("shell");
    }else{
        char c = *nextData;
        if (c==47){
            if((dir = opendir(nextData)) == NULL) {
                printf("%s does not exist.\n",nextData);
            }else{
                strcpy(string, nextData);
                cdir = string;
                printf("%s\n", cdir);
            }
        }else{
            strcpy(string, cdir);
            strcat(string,"/");
            strcat(string, nextData);
            if((dir = opendir(string)) == NULL) {
                printf("%s does not exist.\n",string);
            }else{
                cdir = string;
            }
        }
    }
}

/*
 *Clears away the screen
 */
void clr() {
    printf("\033[2J");
    printf("\033[1;1H");
}

/*
 *displays User and the current directory before asking for input.
 */
void displayCDir() {
    printf("%s",getenv("USER"));
    printf("@");
    printf("%s",cdir);
    printf(":");
}

/*
 *Lists the current directory contents
 */
void dir(char *nextData) {
    char *link;
    DIR *dir;
    struct dirent *file;
    nextData = strtok(NULL, " ");
    if (nextData == NULL){
        printf("%s\n",cdir);
        link = cdir;
    }else{
        link = nextData;
    }
    if ((dir = opendir(link)) == NULL)
        perror("opendir() error");
    while((file = readdir(dir)) != NULL)
        printf("  %s\t", file->d_name);
    printf("\n");
}

void env() {
    extern char **environ;
    int i = 0;
    while (environ[i]){
        printf("%s\n", environ[i++]);
    }
}


void echo(char *nextData) {
    if (nextData == NULL){
        printf("please enter a comment");
    }else{
        nextData = strtok(NULL, " ");
        while(nextData !=NULL){
            printf("%s ", nextData);
            nextData = strtok(NULL, " ");
        }
        printf("\n");
    }
}

void help() {
    printf("#################################### HELP ######################################\n");
    printf("#                          ===== List of Commands =====                        #\n");
    printf("# * cd      - navigates to a specified directory                               #\n");
    printf("# * clr     - clears the current text from the window.                         #\n");
    printf("# * dir     - lists the contents of the current directory.                     #\n");
    printf("# * environ - displays all the environmental variables.                        #\n");
    printf("# * echo    - prints whatever data is given to it.                             #\n");
    printf("# * help    - displays this help text.                                         #\n");
    printf("# * pause   - pauses the shell.                                                #\n");
    printf("# * quit    - terminates the shell. Same as: exit, and kill.                   #\n");
    printf("#################################### HELP ######################################\n");
}


/*
function to set environment values and set starting location.
*/
void setVariables() {
    char cwd[1024];
    char *pathname;
    char *name= "shell";
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        pathname = malloc(strlen(name)+strlen(cwd)+1);
        sprintf(pathname, "%s=%s", name, cwd);
        putenv(pathname);
    }
    else
        perror("getcwd() error");
    return 0;
}

/*
Function that listens for input.
*/
void inputloop() {
    displayCDir();
    char line[256];
    gets(line);
    char *nextData;
    nextData = strtok(line," ");
    while (nextData != NULL)
    {
        if (strcmp("cd", nextData) == 0){
            cd(nextData);
        }else if (strcmp("clr", nextData) == 0) {
            clr();
        }else if (strcmp("dir", nextData) == 0) {
            dir(nextData);
        }else if (strcmp("environ", nextData) == 0) {
            env();
        }else if (strcmp("echo", nextData) == 0) {
            echo(nextData);
        }else if (strcmp("pause", nextData) == 0) {
            printf("Press ENTER to continue.");
            char *a;
            gets(a);
        }else if ((strcmp("quit", nextData) == 0) || (strcmp("exit", nextData) == 0)  || (strcmp("kill", nextData) == 0)) {
            exit(EXIT_SUCCESS);
        }else{
            exec(nextData);
        }
      nextData = strtok(NULL, " ");
      }
}


int main(void)
{
    setVariables();
    cdir = getenv("shell");
    while (1) {
        inputloop();
    }
    return 0;
}
