// Sudoku puzzle verifier and solver

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* structure for passing data to threads */
typedef struct {
    int row;
    int column;
    int (*grid)[9]; // TODO: how to change 9 to any?
} parameters;

// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char *filename, int ***grid);

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int **grid);

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int **grid);

// Checks if the subgrid of the sudoku grid is valid
void *validateSubgrid(void *data) {
  printf("start *validateSubgrid(void *data))\n");

  //int *exitStatus = (int *)malloc(sizeof(int));
  // *exitStatus = EXIT_SUCCESS;

  int digit_check[10] = {0};
  //int size;// = sqrt(psize);
  parameters *prmtrs = (parameters *)data;
  for (int i = prmtrs->row; i < prmtrs->row + 3; i++) { // TDOD: switch 3 to sqrt(psize) ?
  printf("validate subgrid for loop1\n");
    for (int j = prmtrs->column; j < prmtrs->column + 3; j++) {
      printf("validate subGrid for loop2\n");
      if (digit_check[prmtrs->grid[i][j]] == 1) {
        printf("invalid sudoku\n");
        // *exitStatus = EXIT_FAILURE;
        // return (void *)exitStatus;
        return (void *)-1; // Invalid sudoku subgrid
      }
      digit_check[prmtrs->grid[i][j]] = 1;
    }
  }
  //return (void *)exitStatus;
  return (void *)0; // Valid sudoku subgrid
}

// Checks whether the rows of the sudoku grid are valid.
void *validateRows(void *data) {
  printf("start *validateRows(void *data))\n");

  // int *exitStatus = (int*)malloc(sizeof(int));
  // *exitStatus = EXIT_SUCCESS;

  int digit_check[10] = {0};
  parameters *prmtrs = (parameters *)data;
  for (int i = 0; i < 9; i++) { // TODO: change 9 to psize
    for (int j = 0; j < 9; j++) {
      if (digit_check[prmtrs->grid[i][j]] == 1) {
       // *exitStatus = EXIT_FAILURE;
        //return (void *)exitStatus;
        return (void *)-1;
      }
      digit_check[prmtrs->grid[i][j]] = 1;
    }
    memset(digit_check, 0, sizeof(int)*10);
  }
  //return (void *)exitStatus;
  return (void *)0;
}

// Checks whether the columns of the sudoku grid are valid.
void *validateColumns(void *data) {
  printf("start *validateColumns(void *data))\n");

  // int *exitStatus = (int*)malloc(sizeof(int));
  // *exitStatus = EXIT_SUCCESS;

  int digit_check[10] = {0};
  parameters *prmtrs = (parameters *)data;
  for (int i = 0; i < 9; i++) { // TODO: change 9 to psize
    for (int j = 0; j < 9; j++) {
      if (digit_check[prmtrs->grid[j][i]] == 1) {
        // *exitStatus = EXIT_FAILURE;
        // return (void *)exitStatus;
        return (void *)-1;
      }
      digit_check[prmtrs->grid[j][i]] = 1;
    }
    memset(digit_check, 0, sizeof(int)*10);
  }
  //return (void *)exitStatus;
    return (void *)0;
}

// takes puzzle size and grid[][] representing sudoku puzzle
// and tow booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid) {
  // Validate all sudoku grids

  parameters *data[psize];

  int i = 0;
	for(int row = 0; row < psize; row += 3)
	{
		for(int column = 0; column < psize; column += 3, i++)
		{
			data[i] = (parameters *)malloc(sizeof(parameters));
			if(data[i] == NULL){
				int err = errno;
				puts("malloc failed");
				puts(strerror(err));
				exit(EXIT_FAILURE);
			}
			data[i]->row = row;
			data[i]->column = column;
			data[i]->grid = grid;
		}
	}

  int num_threads = psize + 2;  
  pthread_t tid[num_threads];    
  
  int t_status[num_threads], retCode, check_flag = 0;
  for (int g = 1; g <= psize; g++) {
    
    // Create threads for subgrid validation 
    for (int p = 0; p < psize; p++) {
      printf("before validateSubgrid\n");
      if (retCode = pthread_create(&tid[p], NULL, validateSubgrid, (void *)data[p])) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", retCode); 
        exit(EXIT_FAILURE);
      }
    }
    printf("before validateRows\n");
    // Create threads for row and column validation
    if (retCode = pthread_create(&tid[psize+1], NULL, validateRows, (void *)data[0])) {
      fprintf(stderr, "Error - pthread_create() return code: %d\n", retCode); //false);
      exit(EXIT_FAILURE);
    }
    printf("before validateColumns\n");
    if (retCode = pthread_create(&tid[psize+2], NULL, validateColumns, (void *)data[0])) {
      fprintf(stderr, "Error - pthread_create() return code: %d\n", retCode); //false);
      exit(EXIT_FAILURE);
    }
  }

  //pthread_join(tid[0], (void *)&t_status[0]);
    printf("before Join for loop\n");
  //Join all threads so main waits until the threads finish
  for (int j = 0; j < num_threads; j++) {
    void *retVal;
    int join_status;
    join_status = pthread_join(tid[j], &retVal);

  //   // if (join_status != 0) {
  //   //   fprintf(stderr, "Failed to join thread (errorNo : %j)\n", join_status);
  //   //   exit(EXIT_FAILURE);
  //   // }
  //   // int *status = (int*)retVal;


  //   //pthread_join(tid[j], (void *)&t_status[j]);
  //   // if(retCode = pthread_join(tid[j], (void *)&t_status[j])){
  //   //   fprintf(stderr, "Error - pthread_join() return code: %d\n", retCode);
  //   //   exit(EXIT_FAILURE);
  //   // }
  }

  // // check the status returned by each thread
  // for (int h = 0; h < num_threads; h++) {
  //   if(t_status[h]) {
  //     check_flag = 1;
  //     break;
  //   }
  // }
  // if (check_flag) {
  //   printf("Sudoku Puzzle: Invalid\n");
  // } else {
  //   *complete = true;
  //   *valid = true;
  //   printf("Sudoku Puzzle: Valid\n");
  // }


    //*complete = true;

  // if (!complete) { // if complete == false
  //   *valid = false;
  // } else {
  //   *valid = true;
  // }
   
}



// expects file name of the puzzle as argument in command line
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: ./sudoku puzzle.txt\n");
    return EXIT_FAILURE;
  }
  // grid is a 2D array
  int **grid = NULL;
  // find grid size and fill grid
  int sudokuSize = readSudokuPuzzle(argv[1], &grid);
  bool valid = false;
  bool complete = false;
  checkPuzzle(sudokuSize, grid, &complete, &valid);
  printf("Complete puzzle? ");
  printf(complete ? "true\n" : "false\n");
  if (complete) {
    printf("Valid puzzle? ");
    printf(valid ? "true\n" : "false\n");
  }
  printSudokuPuzzle(sudokuSize, grid);
  deleteSudokuPuzzle(sudokuSize, grid);
  return EXIT_SUCCESS;
}

// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char *filename, int ***grid) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  int psize;
  fscanf(fp, "%d", &psize);
  int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
  for (int row = 1; row <= psize; row++) {
    agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
    for (int col = 1; col <= psize; col++) {
      fscanf(fp, "%d", &agrid[row][col]);
    }
  }
  fclose(fp);
  *grid = agrid;
  return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int **grid) {
  printf("%d\n", psize);
  for (int row = 1; row <= psize; row++) {
    for (int col = 1; col <= psize; col++) {
      printf("%d ", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int **grid) {
  for (int row = 1; row <= psize; row++) {
    free(grid[row]);
  }
  free(grid);
}
