// P4: Contiguous Memory Allocation
// This project manage allocations within a memory pool of size MEMSIZE 80.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#define MEMSIZE 80 // Max memory pool

// the name, start, and end memory address of a process
typedef struct Process {
    char name;
    int begin;
    int end;
} process;

// the start and end memory address of a hole
typedef struct Hole {
    int begin;
    int end;
} hole;

char memory[MEMSIZE + 1]; // string representing memory allocation
process processes[MEMSIZE]; // list of processes
hole holes[MEMSIZE]; // list of holes

int numHoles = 1; // number of holes
int allocations = 0; // number of chunks of memory allocated
int maxHole = MEMSIZE; // largest hole size

bool exitStatus = false; // keep going while 'false', program ends when 'true'

void executeCommand(char **command);

// converts alphabetical characters in line to uppercase
void makeUppercase(char *line) {
  int i = 0;
  while (line[i] != 0) {
    if (i == 0 && (line[i] == 'r' || line[i] == 'R')) {
      if (line[i] == 'r') line[i] = 'R';
      break;
    }
    if (line[i] >= 'a' && line[i] <= 'z') {
      line[i] = line[i] - 32;
    }
    i++;
  }
}

// reads command line inputted by user
int  readline(char** buffer) {
  size_t len;   
  int size = getline(buffer, &len, stdin);
   if (size > 0) {
    (*buffer)[size - 1] = '\0';
  }
  return size;
}

// initializes the memory string with "." to be full
void initializeMemory() {
    for (int i = 0; i < MEMSIZE; i++) {
        memory[i] = '.';
    }
    memory[MEMSIZE] = '\0';
    holes[0].begin = 0;
    holes[0].end = MEMSIZE - 1;
}

// separates line into tokens based on spaces
void tokenize(char *line, char **command) {
    char *token, *whitespace = " ";
    int index = 0;
    token = strtok(line, whitespace);
    while (token != NULL) {
        command[index] = token;
        index++;
        token = strtok(NULL, whitespace);
    }
    command[index] = NULL;
}

// returns max int between two numbers
int max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

// updates the holes list and maxHole
void updateHoles() {
    int prev = 0; // beginning address of a new hole
    numHoles = 0, maxHole = INT_MIN;
    // loop over processes
    for (int i = 0; i < allocations; i++) {
        process p = processes[i];
        // if first process's begin address greater than new holes begin
        if (p.begin > prev) {
            hole h = {prev, p.begin - 1};
            holes[numHoles] = h; // add new hole
            numHoles++;
            maxHole = max(h.end - h.begin + 1, maxHole);
        }
        prev = p.end + 1;
    }
    // if next hole's begin isn't end of memory
    if (prev <= MEMSIZE - 1) {
        hole h = {prev, MEMSIZE - 1}; // create new hole for remaining memory
        holes[numHoles] = h;
        numHoles++;
        maxHole = max(h.end - h.begin + 1, maxHole);
    }
}

// allocates process with name and with the specified begin memory address
void allocate(char name, int begin, int size) {
    int wait = 1, i = 0;
    int end = begin + size - 1;
    // loop over memory from begin to end
    for (int i = begin; i <= end; i++) {
        memory[i] = name;
    }
    process p = {name, begin, end};

    // add process to list of processes
    for (i = 0; i < allocations; i++) {
        if (p.begin < processes[i].begin) {
            for (int j = allocations; j > i; j--) {
                processes[j] = processes[j - 1];
            }
            processes[i] = p;
            allocations++;
            wait = 0;
            break;
        }
    }
    // if p comes after every process in list, then add it to end of list
    if (wait) {
        processes[i] = p;
        allocations++;
    }
    updateHoles();
}

// returns 1 if size can fit into the hole
// returns 0 otherwise
int bigEnough(hole h, int size) {
    return (h.end - h.begin + 1) >= size;
}

// allocates memory to process with name using the specified in command algorithm
void request(char name, int size, char algo) {
    if (size > maxHole) {
        printf("Not enough memory\n");
        return;
    }
    if (algo == 'F') { // First-Fit
        for (int i = 0; i < numHoles; i++) {
            hole h = holes[i];
            if (bigEnough(h, size)) {
                allocate(name, h.begin, size);
                break;
            }
        }
    }
    else if (algo == 'B') { // Best-Fit
        int m = INT_MAX; // size of smallest hole
        int minIndex = -1; // begin address of m
        for (int i = 0; i < numHoles; i++) {
            hole h = holes[i];
            if (bigEnough(h, size) && h.end - h.begin + 1 < m) {
                m = h.end - h.begin + 1;
                minIndex = i;
            }
        }
        if (minIndex > -1) { // if minIndex found, then allocate memory
            allocate(name, holes[minIndex].begin, size);
        }
    }
    else if (algo == 'W') { // WorstFit
        for (int i = 0; i < numHoles; i++) {
            hole h = holes[i];
            if (bigEnough(h, size) && h.end - h.begin + 1 == maxHole) {
                allocate(name, holes[i].begin, size);
                break;
            }
        }
    } else {
        printf("Unknown algorithm\n");
    }
}

// free all the allocations owned by <name>
void release(char name) {
    for (int i = 0; i < allocations; i++) {
        process p = processes[i];
        if (p.name == name) {
            for (int i = p.begin; i <= p.end; i++) {
                memory[i] = '.'; // free memory
            }
            for (int j = i; j < allocations; j++) {
                processes[j] = processes[j + 1];
            }
            i--;
            allocations--;
            updateHoles();
        }
    }
}

// compact the memory pool, sliding all allocations to lower addresses 
// so they become one contiguous block,
// and so that all the free space lies to the right as one contiguous block
void compact() {
    int prev = 0;
    for (int i = 0; i < allocations; i++) {
        int size = processes[i].end - processes[i].begin;
        processes[i].begin = prev;
        processes[i].end = processes[i].begin + size;
        prev = processes[i].end + 1;
    }
    initializeMemory();
    for (int i = 0; i < allocations; i++) {
        int begin = processes[i].begin, end = processes[i].end;
        for (int j = begin; j <= end; j++) {
            memory[j] = processes[i].name;
        }
    }
    updateHoles();
}

// read opens a file and executes the command from each line
// assumes file is in correct format
void read(char **command) {
    char line[MEMSIZE];
    FILE *file = fopen(command[1], "r"); // open file
    if (file == NULL) { // file could not be opened
        printf("Could not open file\n");
        return;
    }
    while (fgets(line, sizeof(line), file)) {
        makeUppercase(line);
        tokenize(line, command);
        if (*command[0] == 'E') { // Exit command
            exitStatus = true;
            fclose(file);
        } else {
          executeCommand(command);
       }
    }
}

// execute the command in command buffer
void executeCommand(char **command) {
    if (command[0] == NULL) {
        return;
    }
    // Allocate command, then check for valid args and then execute
    if (*command[0] == 'A' && command[1] != NULL && command[2] != NULL 
        && command[3] != NULL && atoi(command[2]) > 0) {
        request(*command[1], atoi(command[2]), *command[3]);
    }
    else if (*command[0] == 'F' && command[1] != NULL) { // Free memory command
        release(*command[1]);
    }
    else if (*command[0] == 'S') { // Show memory command
        printf("%s", memory);
        printf("\n");
    }
    else if (*command[0] == 'R') { // Read from file command
        read(command);
    }
    else if (*command[0] == 'C') { // Compact command
        compact();
    }
    else if (*command[0] == 'E') { // Exit command
        exitStatus = true;
    }
    else { // Command not found
        printf("Invalid command\n");
    }
}

// main infitely waits for user input and executes command
int main() {
    char *command[MEMSIZE]; // buffer for commands
    char *cmdline = (char *) malloc(MEMSIZE * sizeof(char));
    int size;

    initializeMemory();
    while (1) {
        printf("command>");
        fflush(stdout);
        size = readline(&cmdline); // read command line inputted by user
        if (size > 0) {
            cmdline[size - 1] = 0;
            makeUppercase(cmdline);
            tokenize(cmdline, command);
        }
        executeCommand(command);
        if (exitStatus) {
            printf("Exiting now\n");
            break;
        }
    }
}