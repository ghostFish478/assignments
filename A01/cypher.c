/***************************************************
 * cypher.c
 * Author: Bethany Ho
 * Date: 9/15/2022
 * Makes cypher
 */

#include <stdio.h>
#include <string.h>

/**
 * Function shifts a given word by a given number for
 * the cypher
 * 
 * @param cypher word chosen for the cypher
 * @param shifting number of characters to shift by
 * @return string of characters
 */
char* shift(char* cypher, int shifting) {
  // Length of the word to be cyphered
  int wordLength = strlen(cypher);

  // Loop for each letter in the cypher
  for (int i = 0; i < wordLength; i++) {
    // Shifts word for cypher
    cypher[i] = cypher[i] + shifting;

    // Correction for if cypher shift is greater than 'z' in ASCII
    if (cypher[i] > 'z') {
      cypher[i] = cypher[i] - 26;
    }

    // Correction for if cypher shift is less than 'a' in ASCII
    if (cypher[i] < 'a') {
      cypher[i] = cypher[i] + 26;
    }
  }
  return cypher;
}

int main() {
  // Holds value for an array for a given word
  char oldWord[128];
  // Holds value for shifting the word
  int shiftBy;
  
  printf("Enter a word:\n");
  scanf("%s", oldWord);

  printf("Enter a shift:\n");
  scanf("%d", &shiftBy);

  // Holds value for the shifted word from the function shift
  char* newWord = shift(oldWord, shiftBy);
  printf("Your cypher is %s", newWord);

  return 0;
}