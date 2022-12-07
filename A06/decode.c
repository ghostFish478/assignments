#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

/**
 * Returns 0 and 1 values in allocated memory based
 * on RGB values from a given image
 * 
 * @param canvas the pixels from the old file
 * @param w the width
 * @param h the height
 * @return decodecanvas
 */
int* decode(struct ppm_pixel** canvas, int w, int h) {
  // Creates decodecanvas to hold all binary numbers possible
  int* decodecanvas = malloc(w * h * 3 * sizeof(int));
  // Holds counter for decodecanvas
  int k = 0;
  // Goes through each pixel and sees if it returns a binary value
  // of 1 or 0 based on the RGB value
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      if((int)(canvas[i][j].red) % 2 == 0) {
        decodecanvas[k] = 0;
      } else {
        decodecanvas[k] = 1;
      }
      if((int)(canvas[i][j].green) % 2 == 0) {
        decodecanvas[k+1] = 0;
      } else {
        decodecanvas[k+1] = 1;
      }
      if((int)(canvas[i][j].blue) % 2 == 0) {
        decodecanvas[k+2] = 0;
      } else {
        decodecanvas[k+2] = 1;
      }
      // Increases k counter to go through decodecanvas
      k = k + 3;
    }
  }
  return decodecanvas;
}

/**
 * Returns part of a decimal value
 * 
 * @param bival singular binary value
 * @param powval value for the exponent
 * @return part of a decimal value
 */
int powerdec(int bival, int powval){
  // Holds a part of a decimal value based on a given exponent
  int decimal = 0;
  // Holds value for power of two
  int powertwo = 1;

  // Finds the power of two
  for(int i = 0; i < powval; i++) {
    powertwo = powertwo * 2;
  }
  // Calculates the decimal value
  decimal = bival * powertwo;
  return decimal;
}

/**
 * Converts given binary numbers into ASCII characters
 * 
 * @param givenString memory allocated for binary numbers
 * @param maxchar maximum number of possible characters
 * @param w the width
 * @param h the height
 * @return asciistring
 */
char* asciiconversion(int* givenString, int maxchar, int w, int h) {
  // Initializes decimal value
  int finaldec = 0;
  // Allocates enough memory for the size of the new string
  char* asciistring = malloc(sizeof(char) * maxchar);
  // Holds value for determining exponents
  int power = 7;
  //Holds counter value for going through each asciistringvalue
  int k = 0;

  // Goes through each integer in givenString
  for (int i = 0; i < (w * h * 3); i = i + 8) {
    // Goes through each integer in givenString
    for(int j = 0; j < 8; j++) {
      // Adds each part of the decimal value based upon the given power value
      finaldec += powerdec(givenString[i+j],power);
      // Decreases power by 1
      power = power - 1;
    }

    // Divides decimal value to get the ASCII character decimal value
    // finaldec = finaldec/4;
    // printf("%d\n", finaldec);
    // Casts decimal value into character
    asciistring[k] = (char)(finaldec);

    // Resets value of the decimal
    finaldec = 0;
    // Resets value of power
    power = 7;
    // Increments k
    k++;
  }
  return asciistring;
}

/**
 * Finds the maximum number of possible characters
 * 
 * @param w the width
 * @param h the height
 * @return charnum
 */
int maxchar(int w, int h) {
  // Calculates for max number
  int charnum = (w * h * 3)/8;
  return charnum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  // Holds value for the name of the input file
  char* inputfile = argv[1];
  // Holds width and height, respectively
  int w, h, maximumchar;

  // Gets pixels from the given file; puts it into a structure
  struct ppm_pixel** canvas = read_ppm_2d(inputfile,&w,&h);
  // Gets the binary numbers through the function decode
  int* binarystring = decode(canvas,w,h);
  // Finds the maximum number of possible characters from the image
  maximumchar = maxchar(w,h);
  // Gets the ASCII character phrase by converting the binary into ASCII
  char* asciiphrase = asciiconversion(binarystring,maximumchar,w,h);
  printf("Max number of characters in the image: %d\n", maximumchar);
  printf("%s",asciiphrase);

  for (int k = 0; k < h; k++) {
      free(canvas[k]);
  }
  free(canvas);
  free(binarystring);
  free(asciiphrase);

  return 0;
}