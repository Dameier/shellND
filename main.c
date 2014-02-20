#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputloop() {
    char line[256];
    gets(line);
    char * nextData;
      printf ("Splitting string \"%s\" into tokens:\n",line);
      nextData = strtok(line," ");
      while (nextData != NULL)
      {
        printf("%s\n",nextData);
        if (strcmp("cd", nextData) == 0){
            printf("cd");
        }else if (strcmp("clr", nextData) == 0) {
            printf("clr");
        }else if (strcmp("dir", nextData) == 0) {

        }else if (strcmp("environ", nextData) == 0) {

        }else if (strcmp("help", nextData) == 0) {

        }else if (strcmp("pause", nextData) == 0) {

        }else if (strcmp("quit", nextData) == 0) {
            printf("yep");
            exit(EXIT_SUCCESS);
        }else{printf("not a command");}
        nextData = strtok(NULL, " ");
      }
}

int main(void)
{
    printf("Hello World!\n");
    while (1) {
        inputloop();
    }
    return 0;
}

