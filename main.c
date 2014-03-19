#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

char *cdir;
/*
 *Function that redirects output to a file.
*/
int outRedirect(char *nextData){
    if (nextData == NULL){
        return -1;
    }
    if (strcmp(">", nextData) == 0){
        nextData =strtok(NULL, " \n");
        int file = open(nextData, O_TRUNC|O_CREAT|O_WRONLY,0777);
        if(file < 0)printf("file failed");
        dup2(file,1);
        return file;
    }else if(strcmp(">>", nextData) == 0){
        nextData =strtok(NULL, " \n");
        int file = open(nextData, O_APPEND|O_CREAT|O_WRONLY,0777);
        if(file < 0)printf("file failed");
        dup2(file,1);
        return file;
    }
    return 0;
}

void closeOut(int file, int passed){
//  fflush(stdout);
    close(file);
    //int fl = open(stdout, O_APPEND|O_WRONLY);
    dup2(passed, 1);
  //  dup2(file, fileno(stdout));
    //close(file);
}

/*
 *Function that attempts to execute anything that
 *isn't a native part of our shell.
 */
void exec(char *nextData) {
    int file = -1;
    int passed = dup(1);
    char *envargs[20];
    int count = 0;
    int size = 20;
    while (nextData != NULL) {
       if (count == size) {
            size = size * 2;
            *envargs = (char *)realloc(*envargs, size);
       }
       if (strcmp(nextData,">")==0 || strcmp(nextData,">>") == 0){
           outRedirect(nextData);
           nextData = strtok(NULL, " \n");
           break;
       }
       envargs[count] = nextData;
       count++;
       nextData = strtok(NULL, " \n");
    }
    envargs[count] = (char *)0;
    pid_t pID = fork();
    char * myPath = malloc(snprintf(NULL, 0, "%s %s", "parent=", getenv("shell")) + 1);
    //char *env[] = { "PATH=/usr/bin:/bin:/opt/bin", (char *) 0 };
    char *envp[] = { myPath, (char *) 0};
    if(pID == 0) {
        //execv(envargs[0], envargs);
        printf("\n");
        int execReturn = execve(envargs[0], envargs, envp);
        if (execReturn == -1){
            printf("execv has failed.");
        }
        exit(0);
    }else if(pID < 0){
        printf("\nFailed to fork.");
    }else{
        printf("parent process\n");
    }
    closeOut(file, passed);
}
/*
 *function to change the current working directory.
 *
 *issues: if the user types cd .. it just tacs it on the end.
 *This works but could be done better.
 */
void cd(char *nextData) {
    DIR *dir;
    char *string = malloc(1024);
    nextData = strtok(NULL, " \n");
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
                printf("%s doesn't exist.\n",string);
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
    int passed = dup(1);
    int fl = -1;
    char *link;
    DIR *dir;
    struct dirent *file;
    nextData = strtok(NULL, " \n");
    if (nextData != NULL){
        if (strcmp(nextData,">")==0 || strcmp(nextData,">>") == 0) {
            fl = outRedirect(nextData);
            nextData = strtok(NULL, " \n");
        }
    }
    if (nextData == NULL){
        printf("%s\n",cdir);
        link = cdir;
    }else{
        link = nextData;
        nextData = strtok(NULL, " \n");
        outRedirect(nextData);
    }
    if ((dir = opendir(link)) == NULL)
        perror("opendir() error");
    while((file = readdir(dir)) != NULL)
        printf("  %s\t", file->d_name);
    printf("\n");
    closeOut(fl, passed);
}

void env(char *nextData) {
    nextData = strtok(NULL," \n");
    int file = -1;
    int passed = dup(1);
    file = outRedirect(nextData);
    extern char **environ;
    int i = 0;
    while (environ[i]){
        printf("%s\n", environ[i++]);
    }
    closeOut(file, passed);
}


void echo(char *nextData) {
    int passed = dup(1);
    int file = -1;
    char *oldData = malloc(100);
    nextData = strtok(NULL, " \n");
    if (nextData == NULL){
        printf("please enter a comment");
    }else{
        while(nextData !=NULL){
            strcat(oldData, nextData);
            strcat(oldData," ");
            nextData = strtok(NULL, " \n");
            if (nextData != NULL){
                if (strcmp(nextData,">")==0 || strcmp(nextData,">>") == 0) {
                    file = outRedirect(nextData);
                    nextData = strtok(NULL, " \n");
                }
            }
        }
        printf("%s", oldData);
        printf("\n");
    }
    closeOut(file, passed);
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
void inputloop(char *ln) {
    displayCDir();
    char *nextData;
    if(ln == NULL){
        char line[256];
        gets(line);
        nextData = strtok(line," \n");
    }else{
        nextData = strtok(ln," \n");
    }
    if (nextData != NULL)
    {
        if (strcmp("cd", nextData) == 0){
            cd(nextData);
        }else if (strcmp("clr", nextData) == 0) {
            clr();
        }else if (strcmp("dir", nextData) == 0) {
            dir(nextData);
        }else if (strcmp("environ", nextData) == 0) {
            env(nextData);
        }else if (strcmp("echo", nextData) == 0) {
            echo(nextData);
        }else if (strcmp("help", nextData) == 0){
            nextData = strtok(NULL, " \n");
            int currentOut = dup(1);
            int file = outRedirect(nextData);
            help();
            closeOut(file,currentOut);
        }else if (strcmp("pause", nextData) == 0) {
            printf("Press ENTER to continue.");
            char *a;
            gets(a);
        }else if ((strcmp("quit", nextData) == 0) || (strcmp("exit", nextData) == 0)  || (strcmp("kill", nextData) == 0)) {
            exit(EXIT_SUCCESS);
        }else{
            exec(nextData);
        }
      nextData = strtok(NULL, " \n");
      }
}


int main(int argc, char* argv[])
{
    setVariables();
    cdir = getenv("shell");
    if (argv[1] != NULL){
        char line[256];
        FILE *file = fopen(argv[1], "r");
        while (fgets(line, sizeof(line), file) !=NULL) {
            inputloop(line);
        }
    }else{
        while (1) {
            inputloop(NULL);
        }
    }
    return 0;
}
