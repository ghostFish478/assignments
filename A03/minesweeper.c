/***************************************************
 * minesweeper.c
 * Author: Bethany Ho
 * Date: 10/1/2022
 * Makes two minesweeper grids; one unsolved and one solved
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Returns a bomb or a space given a probability
 * 
 * @param prob the probability
 * @return 'X' or '.'
 */
char bombs(double prob) {
  // Generates a random integer
  float random = ((float)rand())/RAND_MAX;
  // Returns a bomb if random is under the given probability
  if (random < prob) {
    return 'X';
  }
  // Returns a space otherwise
  return '.';
}

/**
 * Fills the matrix for minesweeper with bombs
 * and spaces; prints them out
 * 
 * @param minematrix the empty matrix for minesweeper
 * @param mrow the number of rows
 * @param ncolumn the number of columns
 * @param probability the probability
 * @return new minematrix
 */
char* fillMatrix(char* minematrix, int mrow, int ncolumn, double probability) {
  // Goes through each element in the minematrix and puts in a space or a bomb
  // Prints out each element into a grid
  for (int i = 0; i < (mrow * ncolumn); i = i + ncolumn) {
    minematrix[i] = bombs(probability);
    printf("%c ", minematrix[i]);
    for (int j = 1; j < ncolumn; j++) {
      minematrix[i+j] = bombs(probability);
      printf("%c ", minematrix[i+j]);
    }
    printf("%c", '\n');
  }
  return minematrix;
}

/**
 * Fills the matrix for minesweeper with numbers
 * in the empty spaces given the bombs already
 * existent
 * 
 * @param minematrix old mine matrix
 * @param space given number space in the allocated memory
 * @param mrow the number of rows
 * @param ncolumn the number of columns
 * @return number of bombs around a space
 */
char checkBombs(char* minematrix, int space, int mrow, int ncolumn) {
  // Holds value for number of bombs around a given space
  int check = 0;
  // Checks for first corner space
  if (space == 0) {
    // Checks E bomb
    if (minematrix[space + 1] == 'X') {
      check++;
    }
    // Checks S bomb
    if (minematrix[space + ncolumn] == 'X') {
      check++;
    }
    // Checks SE bomb
    if (minematrix[space + ncolumn + 1] == 'X') {
      check++;
    }
  }
  // Checks for second corner space
  else if (space == ncolumn - 1) {
    // Checks W bomb
    if (minematrix[space - 1] == 'X') {
      check++;
    }
    // Checks SW bomb
    if (minematrix[space + ncolumn - 1] == 'X') {
      check++;
    }
    // Checks S bomb
    if (minematrix[space + ncolumn] == 'X') {
      check++;
    }
  }
  // Checks for third corner space
  else if(space == (ncolumn * mrow - ncolumn)) {
    // Checks N bomb
    if (minematrix[space - ncolumn] == 'X') {
      check++;
    }
    // Checks NE bomb
    if (minematrix[space - ncolumn + 1] == 'X') {
      check++;
    }
    // Checks E bomb
    if (minematrix[space + 1] == 'X') {
      check++;
    }
  }
  // Checks for fourth corner space
  else if (space == (ncolumn * mrow - 1)) {
    // Checks NW bomb
    if (minematrix[space - ncolumn - 1] == 'X') {
      check++;
    }
    // Checks N bomb
    if (minematrix[space - ncolumn] == 'X') {
      check++;
    }
    // Checks W bomb
    if (minematrix[space - 1] == 'X') {
      check++;
    }
  }
  // Checks for first row edge cases
  else if (space < ncolumn && space > 0) {
    // Checks W bomb
    if (minematrix[space - 1] == 'X') {
      check++;
    }
    // Checks E bomb
    if (minematrix[space + 1] == 'X') {
      check++;
    }
    // Checks SW bomb
    if (minematrix[space + ncolumn - 1] == 'X') {
      check++;
    }
    // Checks S bomb
    if (minematrix[space + ncolumn] == 'X') {
      check++;
    }
    // Checks SE bomb
    if (minematrix[space + ncolumn + 1] == 'X') {
      check++;
    }
  }
  // Checks for bottom row edge cases
  else if (space > (ncolumn - 1) * mrow && space < ncolumn * mrow) {
    // Checks NW bomb
    if (minematrix[space - ncolumn - 1] == 'X') {
      check++;
    }
    // Checks N bomb
    if (minematrix[space - ncolumn] == 'X') {
      check++;
    }
    // Checks NE bomb
    if (minematrix[space - ncolumn + 1] == 'X') {
      check++;
    }
    // Checks W bomb
    if (minematrix[space - 1] == 'X') {
      check++;
    }
    // Checks E bomb
    if (minematrix[space + 1] == 'X') {
      check++;
    }
  }
  // Checks for right edge cases
  else if (space % ncolumn == ncolumn - 1) {
    // Checks NW bomb
    if (minematrix[space - ncolumn - 1] == 'X') {
      check++;
    }
    // Checks N bomb
    if (minematrix[space - ncolumn] == 'X') {
      check++;
    }
    // Checks W bomb
    if (minematrix[space - 1] == 'X') {
      check++;
    }
    // Checks SW bomb
    if (minematrix[space + ncolumn - 1] == 'X') {
      check++;
    }
    // Checks S bomb
    if (minematrix[space + ncolumn] == 'X') {
      check++;
    }
  }
  // Checks for left edge cases
  else if (space % ncolumn == 0) {
    // Checks N bomb
    if (minematrix[space - ncolumn] == 'X') {
      check++;
    }
    // Checks NE bomb
    if (minematrix[space - ncolumn + 1] == 'X') {
      check++;
    }
    // Checks E bomb
    if (minematrix[space + 1] == 'X') {
      check++;
    }
    // Checks S bomb
    if (minematrix[space + ncolumn] == 'X') {
      check++;
    }
    // Checks SE bomb
    if (minematrix[space + ncolumn + 1] == 'X') {
      check++;
    }
  }
  // Checks for all other cases
  else {
    // Checks NW bomb
    if (minematrix[space - ncolumn - 1] == 'X') {
      check++;
    }
    // Checks N bomb
    if (minematrix[space - ncolumn] == 'X') {
      check++;
    }
    // Checks NE bomb
    if (minematrix[space - ncolumn + 1] == 'X') {
      check++;
    }
    // Checks W bomb
    if (minematrix[space - 1] == 'X') {
      check++;
    }
    // Checks E bomb
    if (minematrix[space + 1] == 'X') {
      check++;
    }
    // Checks SW bomb
    if (minematrix[space + ncolumn - 1] == 'X') {
      check++;
    }
    // Checks S bomb
    if (minematrix[space + ncolumn] == 'X') {
      check++;
    }
    // Checks SE bomb
    if (minematrix[space + ncolumn + 1] == 'X') {
      check++;
    }
  }
  return check + '0';
}

/**
 * Fills the matrix for minesweeper with numbers
 * in the empty spaces given the bombs already
 * existent
 * 
 * @param minematrix old mine matrix
 * @param mrow the number of rows
 * @param ncolumn the number of columns
 * @return new mine matrix
 */
char* solveMatrix(char* minematrix, int mrow, int ncolumn) {
  // Goes through each value in the minematrix
  for (int i = 0; i < (mrow * ncolumn); i++) {
    // Checks if there is an empty space to see how many bombs are around
    // it with checkBombs()
    if(minematrix[i] == '.') {
      minematrix[i] = checkBombs(minematrix, i, mrow, ncolumn);
    }
  }

  // Prints out each value of the minematrix in the given grid format
  for (int x = 0; x < (mrow * ncolumn); x++) {
    if (x % ncolumn == 0) {
      printf("%c", '\n');
      printf("%c ", minematrix[x]);
    } else {
      printf("%c ", minematrix[x]);
    }
  }

  return minematrix;
}

int main() {
  srand(time(0));
  // mgrid holds value for m-rows
  int mgrid;
  // ngrid holds value for n-columns
  int ngrid;
  // prob holds value for probability
  double prob;

  // Scan all inputs for minesweeper
  scanf("%d %d %le", &mgrid, &ngrid, &prob);

  // Allocate memory towards an empty matrix
  char* emptyMatrix = malloc(((mgrid * ngrid) + 1) * sizeof(char));

  // Fill minesweep matrix with bombs and spaces
  char* mineSweep = fillMatrix(emptyMatrix, mgrid, ngrid, prob);

  printf("%c", '\n');

  // Fill and print minesweep matrix with numbers based on bombs
  solveMatrix(mineSweep, mgrid, ngrid);

  free(mineSweep);

  return 0;
}