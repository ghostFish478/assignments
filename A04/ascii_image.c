/***************************************************
 * ascii_image.c
 * Author: Bethany Ho
 * Date: 10/9/2022
 * Prints the associated ASCII characters for a PPM file
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

/**
 * Returns an ASCII character
 * 
 * @param redI value for Red
 * @param greenI value for Green
 * @param blueI value for Blue
 * @return '@', '#', '%', '*', 'o', ';', ':', ',', '.', or ' '
 */
char assignASCII(unsigned char redI, unsigned char greenI, unsigned char blueI) {
  // Convers each of the unsigned chars into integers
  redI = (int)redI;
  greenI = (int)greenI;
  blueI = (int)blueI;

  // Calculates intensity based on the given RGB values
  int intensity = (redI + greenI + blueI)/3;

  // Returns character value depending on the calculated intensity
  if(intensity >= 0 && intensity <= 25) {
    return '@';
  }
  else if(intensity >= 26 && intensity <= 50) {
    return '#';
  }
  else if(intensity >= 51 && intensity <= 75) {
    return '%';
  }
  else if(intensity >= 76 && intensity <= 100) {
    return '*';
  }
  else if(intensity >= 101 && intensity <= 125) {
    return 'o';
  }
  else if(intensity >= 126 && intensity <= 150) {
    return ';';
  }
  else if(intensity >= 151 && intensity <= 175) {
    return ':';
  }
  else if(intensity >= 176 && intensity <= 200) {
    return ',';
  }
  else if(intensity >= 201 && intensity <= 225) {
    return '.';
  }
  else if(intensity >= 226 && intensity <= 255) {
    return ' ';
  }
  return '.';
}

int main(int argc, char** argv) {
  // Holds value for the name of the input file
  const char* inputfile = argv[1];
  // Holds value for a character associated with a RGB intensity
  char intensityASCII;
  // Holds value for width, and height
  int w, h;

  // Statement for incorrect number of arguments given
  if (argc != 2) {
    printf("usage: ascii_image <file.ppm>\n");
    return 0;
  }

  // Holds value for a read ppm 2D array based on the given file name, width, and height
  struct ppm_pixel** ppmarray = read_ppm_2d(inputfile, &w, &h);

  // Print out each character in the ppm array based on intensity calculation
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      // Calculates the overall intensity and the associated character
      intensityASCII = assignASCII(ppmarray[i][j].red, ppmarray[i][j].green, ppmarray[i][j].blue);
      printf("%c", intensityASCII);
    }
    printf("\n");
  }

  for (int k = 0; k < h; k++) {
    free(ppmarray[k]);
  }
  free(ppmarray);
  return 0;
}