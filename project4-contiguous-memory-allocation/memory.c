
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MEMSIZE 80 // max size of a memory pool

char *pool[MEMSIZE]; // create an array to represent a memory pool

// reads command line inputted by user
int readline(char** buffer) 
{
  size_t len;   
  int number_of_chars = getline(buffer, &len, stdin);
   if (number_of_chars > 0)
  {
    (*buffer)[number_of_chars - 1] = '\0';
  }
  return number_of_chars;
}

// separates line into tokens based on spaces 
int tokenize(char* line, char** tokens)
{
  char* pch;
  pch =  strtok(line, " ");
  int num = 0;
  while (pch != NULL)
  {
    tokens[num] = pch;
    ++num;
    pch =  strtok(NULL, " ");
  }
  return num;
}

// initialize the array
void initialize() {
  for(int i = 0; i < MEMSIZE; i++) {
    pool[i] = ".";
  }
}

void allocate(char *name, int size, char *algo) {
  printf("allocate\n");
}

// read the text file line by line and execute the commands
void executeCommands(char *file) {
  printf("executeCommands2\n");
  // FILE *in = fopen(file, "r");
  
}

void show() {
  for(int i = 0; i < MEMSIZE; i++) {
    printf("%s", pool[i]);
  }
  printf("\n");
}


int main(void) {
  initialize();

  char *args[MEMSIZE/2 + 1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit program */

  char *cmdline = (char *) malloc(MEMSIZE * sizeof(char));

  while (should_run) {
    fflush(stdout);
    int len = readline(&cmdline);
    
    for (int i = 0; i < MEMSIZE/2 + 1; ++i)
    {
      args[i] = NULL;
    }
    int num_of_tokens = tokenize(cmdline, args);

    if (strcmp(cmdline, "") == 0) {
      continue; 
    }

    if (strcmp(cmdline, "A") == 0) {
      printf("Allocate\n");
    }
    else if (strcmp(cmdline, "F") == 0) {
      printf("Free\n");
    }
    else if (strcmp(cmdline, "S") == 0) {
      printf("Show\n");
      show();
    }
    else if (strcmp(cmdline, "C") == 0) {
      printf("Compact\n");
        }
    else if (strcmp(cmdline, "R") == 0) {
          printf("Read\n");
        }
    else if (strcmp(cmdline, "E") == 0) {
          printf("Exit\n");
          break;
        }
    else {
          printf("Please enter a valid command!\n");
        }
  }
}