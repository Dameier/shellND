#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

/*
Function that listens for input.
*/
void inputloop() {
    char line[256];
    gets(line);
    DIR *dir;
    struct dirent *file;
    char * nextData;
      nextData = strtok(line," ");
      while (nextData != NULL)
      {
        printf("%s\n",nextData);
        if (strcmp("cd", nextData) == 0){
            printf("cd\n");
        }else if (strcmp("clr", nextData) == 0) {
            printf("clr\n");
        }else if (strcmp("dir", nextData) == 0) {
            nextData = strtok(NULL, " ");
            if (nextData == NULL){
                nextData = "/";
            }
            if ((dir = opendir(("%s", nextData))) == NULL)
                perror("opendir() error");
            while((file = readdir(dir)) != NULL)
                printf("  %s\t", file->d_name);
            printf("\n");
        }else if (strcmp("environ", nextData) == 0) {

        }else if (strcmp("help", nextData) == 0) {

        }else if (strcmp("pause", nextData) == 0) {

        }else if (strcmp("quit", nextData) == 0) {
            printf("yep");
            exit(EXIT_SUCCESS);
        }else{printf("not a command\n");}
        nextData = strtok(NULL, " ");
      }
}

/* function to set environment values and set starting location.

*/
void setVariables() {

}

int main(void)
{
    setVariables();
    while (1) {
        inputloop();
    }
    return 0;
}
