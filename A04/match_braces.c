/***************************************************
 * match_braces.c
 * Author: Bethany Ho
 * Date: 10/9/2022
 * Puts braces into a stack and determines whether it has a
 * matching brace or not
 */

#include <stdio.h>
#include <stdlib.h>

// Structure for the stack
struct stack {
  // Holds value for the location of the top element of the stack
  int top;
  // Holds value for maximum size of the stack
  int max_size;
  // Holds value for structure for holding values related to the left brace
  struct brace* braces;
};

// Structure for the values relating to the left brace in the stack
struct brace {
  // Holds value for line number
  int lineno;
  // Holds value for column number
  int col;
  // Holds value for a character
  char c;
};

/**
 * Initializes a new stack
 * 
 * @param filename the contents of a file
 * @return newStack
 */
struct stack* initStack(FILE *filename) {
  struct stack* newStack = malloc(sizeof(filename) * sizeof(struct stack)); // Might need to fix this
  // Initialize the top of the stack
  newStack->top = -1;
  // Initialize the size of the stack
  newStack->max_size = 0;
  // struct brace* newStack->braces = malloc(sizeof(filename) * sizeof(struct brace*));
  return newStack;
}

/**
 * Pushes new value onto a stack
 * 
 * @param holdStack the name of the stack
 * @param line the line the left brace is associated with
 * @param column the column number the brace is associated with
 * @param char the left brace character that will be added onto the stack
 * @return holdStack
 */
struct stack* push(struct stack* holdStack, int line, int column, char givenchar) {
  // Change top value to accomodate for new added value
  holdStack->top = holdStack->top + 1;
  // Increase the stack size
  holdStack->max_size = holdStack->max_size + 1;
  // Assign lineno, col, and c with the given line, column, and givenchar for the new element
  holdStack->braces[holdStack->top].lineno = line;
  holdStack->braces[holdStack->top].col = column;
  holdStack->braces[holdStack->top].c = givenchar;
  return holdStack;
}

/**
 * Removes the top value from a stack
 * 
 * @param holdStack the name of the stack
 * @return holdStack
 */
struct stack* pop(struct stack* holdStack) {
  // holdStack->braces[holdStack->top].lineno = NULL;
  // holdStack->braces[holdStack->top].col = NULL;
  // holdStack->braces[holdStack->top].c = NULL;
  // Changes to new top value
  holdStack->top = holdStack->top - 1;
  // Decreases max size
  holdStack->max_size = holdStack->max_size - 1;
  return holdStack;
}

/**
 * Removes the entire stack
 * 
 * @param holdStack the name of the stack
 */
void clear(struct stack* holdStack) {
  // For each element in the stack, pop the stack
  for(int i = 0; i < holdStack->max_size; i++) {
    pop(holdStack);
  }
  free(holdStack);
}

int main(int argc, char** argv) {
  // Holds value for vile
  FILE *infile;
  // Holds value for the stack based on the file
  struct stack* fileStack;
  // Holds value for location in a file
  int line, column;
  // Holds value for a character
  char character;

  // Error message for incorrect number of arguments
  if (argc != 2) {
    printf("usage: \\.%s\n", argv[1]);
    exit(0);
  }

  // Opens file
  infile = fopen(argv[1], "r");
  // Error message for missing file
  if (infile == NULL) {
    printf("Cannot open file: %s\n", argv[1]);
    exit(1);
  }

  // Initializes value for line number
  line = 1;
  // Initializes value for column number
  column = 0;
  // Initializes stack
  fileStack = initStack(infile);

  // Gets each character from the file until the end of the file
  while((character = fgetc(infile)) != EOF) {
    // Goes through each column to check for a character
    column = column + 1;
    // If '\n' is found, moves to the next line and resets column to 1
    if (character == '\n') {
      line = line + 1;
      column = 1;
    }
    // Pushes into stack if left bracket is found
    if(character == '{') {
      fileStack = push(fileStack, line, column, '{');
    }
    // Pops stack and prints if right bracket is found
    if(character == '}') {
      printf("Found matching braces: (%d, %d) -> (%d, %d)", fileStack->braces[fileStack->top].lineno, fileStack->braces[fileStack->top].col, line, column);
      fileStack = pop(fileStack);
    }
  }

  // Checks and prints for unmatched left brace
  if(fileStack->braces[fileStack->top].c == '{' && fileStack->max_size == 1) {
    printf("Unmatched brace on Line %d and Column %d", fileStack->braces[fileStack->top].lineno, fileStack->braces[fileStack->top].col);
  }

  clear(fileStack);
  fclose(infile);

  return 0;
}