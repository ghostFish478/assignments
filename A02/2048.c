/***************************************************
 * 2048.c
 * Author: Bethany Ho
 * Date: 9/22/2022
 * Returns the highest value for a shift in 2048
 */

#include <stdio.h>

/**
 * Shifts the board horizontally and retrieves
 * the highest value from such a shift
 * 
 * @param matrix
 * @return highest value from horizontal shift
 */
int shiftMatrixLeft(int matrix[4][4]) {
  // Holds highest value from a shift
  int highestVal = 0;
  // Goes through each value, row-by-row
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      // Checks if two spaces have numbers
      if(matrix[i][j] != 0 && matrix[i][j+1] != 0) {
        // Checks if two numbers in a row are the same
        if(matrix[i][j] == matrix[i][j+1]) {
          // Replaces value of number with new value if it is the same
          matrix[i][j] = 2 * matrix[i][j];
          // Removes the old value
          matrix[i][j+1] = 0;

          // Checks if the new value is higher than the highest value; replaces if so
          if (matrix[i][j] > highestVal) {
            highestVal = matrix[i][j];
          }
        }
      }

      // Skips empty spaces to check against other numbers in the same row
      else if (matrix[i][j] != 0 && matrix[i][j+1] == 0) {
        for(int k = 2; k < (4-j); k++) {
          // Checks if space has a number
          if(matrix[i][j+k] != 0) {
            // Checks if a number is equal to the number after the empty spaces
            if(matrix[i][j] == matrix[i][j+k]) {
              // Replaces value of number with new value if it is the same
              matrix[i][j] = 2 * matrix[i][j];
              // Removes the old value
              matrix[i][j+k] = 0;

              // Checks if the new value is higher than the highest value; replaces if so
              if (matrix[i][j] > highestVal) {
                highestVal = matrix[i][j];
              }
            }
            break;
          }
        }
      }
    }
  }
  printf("%d\n", highestVal);
  return highestVal;
}

/**
 * Shifts the board vertically and retrieves
 * the highest value from such a shift
 * 
 * @param matrix
 * @return highest value from vertical shift
 */
int shiftMatrixUp(int matrix[4][4]) {
  // Holds highest value from a shift
  int highestVal = 0;
  // Goes through each value in the matrix, column-by-column
  for(int j = 0; j < 3; j++) {
    for(int i = 0; i < 3; i++) {
      // Checks if two spaces have numbers
      if(matrix[i][j] != 0 && matrix[i+1][j] != 0) {
        // Checks if two numbers in a column are the same
        if(matrix[i][j] == matrix[i+1][j]) {
          // Replaces value of number with new value if it is the same
          matrix[i][j] = 2 * matrix[i][j];
          // Removes the old value
          matrix[i+1][j] = 0;
          // Checks if the new value is higher than the highest value; replaces if so
          if (matrix[i][j] > highestVal) {
            highestVal = matrix[i][j];
          }
        }
      }

      // Skips empty spaces to check against other numbers in the same column
      else if (matrix[i][j] != 0 && matrix[i+1][j] == 0) {
        for(int k = 2; k < (4-i); k++) {
          // Checks if space has a number
          if(matrix[i+k][j] != 0) {
            // Checks if a number is equal to the number after the empty spaces
            if(matrix[i][j] == matrix[i+k][j]) {
              // Replaces value of number with new value if it is the same
              matrix[i][j] = 2 * matrix[i][j];
              // Removes the old value
              matrix[i+k][j] = 0;
              // Checks if the new value is higher than the highest value; replaces if so
              if (matrix[i][j] > highestVal) {
                highestVal = matrix[i][j];
              }
            }
            break;
          }
        }
      }
    }
  }
  printf("%d\n", highestVal);
  return highestVal;
}

int main() {
  // Holds value for the game board
  int gameGrid[4][4];
  // Holds value for reading in integers from game board
  int val;
  // Holds highest value that can be found from a horizontal shift
  int horizontalShift;
  // Holds highest value that can be found from a vertical shift
  int verticalShift;

// Scans each value from a given game board into a matrix
for (int i = 0; i < 4; i++) {
  for(int j = 0; j < 4; j++) {
    scanf(" %d", &val);
    gameGrid[i][j] = val;
  }
}

  // Assigns highest value from a horizontal shift
  horizontalShift = shiftMatrixLeft(gameGrid);

  // Assigns highest value from a vertical shift
  verticalShift = shiftMatrixUp(gameGrid);

  // Outputs highest value for game board depending on whether
  // horizontal shift or vertical shift is higher
  if (horizontalShift > verticalShift) {
    printf("The largest value is %d.\n", horizontalShift);
  }
  else if (verticalShift > horizontalShift) {
    printf("The largest value is %d.\n", verticalShift);
  }
  else if (verticalShift == horizontalShift) {
    printf("The largest value is %d.\n", horizontalShift);
  }
  return 0;
}
