#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
 * Encodes the message within the new .ppm file
 * 
 * @param canvas the image from the old file
 * @param binary the string of binary numbers
 * @param w the width
 * @param h the height
 * @return canvas
 */
struct ppm_pixel** encode(struct ppm_pixel** canvas, char* binary, int w, int h) {
  // Holds counter for going through binary numbers
  int i = 0;
  // Holds counter for going through canvas height
  int j = 0;
  // Holds counter for going through canvas width
  int k = 0;

  // Goes through each character in the binary string and assigns it to RGB values
  while(i < strlen(binary)) {
    canvas[j][k].red = binary[i];
    if(i + 1 < strlen(binary)) {
      canvas[j][k].green = binary[i + 1];
    }
    if(i + 2 < strlen(binary)) {
      canvas[j][k].blue = binary[i + 2];
    }

    // Increments counters for binary number, height, width;
    i++;
    j++;
    k++;
    // Checks if k needs to be reset
    if (k > w) {
      k = 0;
    }
  }
  return canvas;
}

/**
 * Converts a phrase into a string of binary numbers
 * 
 * @param phrase the given phrase
 * @return binaryarray
 */
char* convertbi(char* phrase) {
  // Allocates memory to hold the binary values of the given characters
  char* binaryarray = malloc(strlen(phrase) * sizeof(int) * 8);
  // Holds values for a given character, and the character with the bit mask
  char letter, maskletter;
  // Mask for getting left-most bit
  unsigned char leftmask = 0x1ul;
  // Holds counter for binary array
  int k;

  // Loops through each character in phrase
  for(int i = 0; i < strlen(phrase); i++) {
    // j is used to determine the shift for letter
    for(int j = 0; j < 8; j++) {
      // Holds character value
      letter = phrase[i];
      // Bit mask for left-most bit
      leftmask = 0x1ul << 7;
      // Shifts letter
      letter = letter << j;
      // Used to see if maskletter is equal to 0 or not
      // based on letter and leftmask
      maskletter = letter & leftmask;
      if(maskletter == 0) {
        binaryarray[k] = '0';
      } else {
        binaryarray[k] = '1';
      }
      // Increases counter to go through binary array
      k++;
    }
  }
  return binaryarray;
}

/**
 * Finds the maximum number of possible characters
 * 
 * @param w the width
 * @param h the height
 * @return charnum
 */
int maxchar(int w, int h) {
  // Caclculates for max number
  int charnum = (w * h * 3)/8;
  return charnum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  // Holds value for the name of the input file
  const char* inputfile = argv[1];
  // Holds width and height, respectively
  int w, h, maximumchar;
  // Holds array for a possible phrase
  char phrase[128];

  // Holds value for pixels from given image
  struct ppm_pixel** canvas = read_ppm_2d(inputfile,&w,&h);

  // Finds maximum number of characters
  maximumchar = maxchar(w,h);
  printf("Max number of characters in the image: %d\n", maximumchar);

  printf("Enter a phrase: ");
  // Holds value for given phrase
  scanf("%s",phrase);

  // Converts phrase into binary
  char* binaryarray = convertbi(phrase);
  // Uses binary values for a new image
  struct ppm_pixel** newcanvas = encode(canvas,binaryarray,w,h);

  // Holds value for a new string for the new file name for the outfile
  char newfilename[sizeof(inputfile) + 12 * sizeof(char)];
  // Shortens original file name for new file name
  strncpy(newfilename,inputfile,strlen(inputfile)-4);
  // Appends 0 to the shortened original file name
  newfilename[strlen(inputfile)-5] = '\0';
  // Creates the new file name
  strcat(newfilename,"-encoded.ppm");

  // Write new file
  write_ppm_2d(newfilename,newcanvas,w,h);

  return 0;
}

