/***************************************************
 * slow.c
 * Author: Bethany Ho
 * Date: 9/22/2022
 * Gives a new slowed-down phrase
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Checks the time and compares it with the opening
 * and closing time to see if a given restaurant is
 * open.
 * 
 * @param pauseLength length of pause
 * @param textPhrase given text
 * @param newTextArray array for the new text
 * @return new text
 */
char* slow(int pauseLength, char *textPhrase, char *newTextArray) {
  // Holds value for counter for changing value of newTextArray
  int i = 0;
  // Loops through each character in the text phrase
  for (int j = 0; j < strlen(textPhrase); j++) {
    // Assigns value from a character in textPhrase to the newTextArray
    newTextArray[i] = textPhrase[j];
    // Adds pause length depending on which letters have already been added
    // to the text phrase
    for (int k = 1; k < pauseLength + 1; k++) {
      newTextArray[i + k] = '.';
    }
    // Updates location for the next character in textPhrase
    i = i + pauseLength + 1;
  }
  return newTextArray;
}

int main() {
  // Holds the string for the given text
  char phrase[32];
  // Holds the length of pause
  int pause;
  // Holds the value for the array for the new string
  char *textArray = NULL;

  printf("Pause Length:\n");
  scanf("%d", &pause);

  printf("Text:\n");
  scanf("%s", phrase);

  // Allocates memory to textArray based on the length of the given text and the pause length
  textArray = malloc((pause * (strlen(phrase) + 1)) + strlen(phrase) + 1);
  memset(textArray, 0, (pause * (strlen(phrase) + 1)) + strlen(phrase) + 1);

  // Gives new text array value to entText based on slow function
  slow(pause, phrase, textArray);

  printf("%s\n", textArray);
  free(textArray);

  return 0;
}