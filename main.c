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
        if ("cd" == nextData){

        }else if (*nextData == "clr") {

        }else if ("dir" == *nextData) {

        }else if (*nextData == "environ") {

        }else if (*nextData == "help") {

        }else if (*nextData == "pause") {

        }else if (nextData == "quit") {
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

