#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void cd(char *nextData) {

}

void clr() {

}

void dir(char nextData) {
    DIR *dir;
    struct dirent *file;
    nextData = strtok(NULL, " ");
    if (nextData == NULL){
        nextData = "/";
    }
    if ((dir = opendir(("%s", nextData))) == NULL)
        perror("opendir() error");
    while((file = readdir(dir)) != NULL)
        printf("  %s\t", file->d_name);
    printf("\n");
}

void environ() {

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

}

void mypause() {

}

/* function to set environment values and set starting location.

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

    char line[256];
    gets(line);
    char * nextData;
      nextData = strtok(line," ");
      while (nextData != NULL)
      {
        if (strcmp("cd", nextData) == 0){
            cd(nextData);
        }else if (strcmp("clr", nextData) == 0) {
            printf("clr\n");
        }else if (strcmp("dir", nextData) == 0) {
            dir(nextData);
        }else if (strcmp("environ", nextData) == 0) {

        }else if (strcmp("echo", nextData) == 0) {
            echo(nextData);
        }else if (strcmp("help", nextData) == 0) {

        }else if (strcmp("pause", nextData) == 0) {

        }else if (strcmp("quit", nextData) == 0) {
            exit(EXIT_SUCCESS);
        }else{printf("not a command\n");}
        nextData = strtok(NULL, " ");
      }
}


int main(void)
{
    setVariables();
    while (1) {
        inputloop();
    }
    return 0;
}
