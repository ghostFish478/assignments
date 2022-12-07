#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
 * Creates new color value for a pixel
 * 
 * @param oldcolorvalue given RGB value
 * @return newcolorvalue
 */
unsigned long minglitch(unsigned long oldcolorvalue) {
  // Shifts bit values for the new color value
  unsigned long newcolorvalue = oldcolorvalue << (rand() % 2);
  return newcolorvalue;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: glitch <file.ppm>\n");
    return 0;
  }

  // Holds value for the name of the input file
  const char* inputfile = argv[1];
  // Holds width and height, respectively
  int w, h;
  // Holds value for a new string for the new file name for the outfile
  char newfilename[sizeof(inputfile) + 11 * sizeof(char)];
  // Shortens original file name for new file name
  strncpy(newfilename,inputfile,strlen(inputfile)-3);
  // Appends 0 to the shortened original file name
  newfilename[strlen(inputfile)-4] = '\0';
  // Creates the new file name
  strcat(newfilename,"-glitch.ppm");

  // Reads original file and puts it into canvas
  struct ppm_pixel** canvas = read_ppm_2d(inputfile,&w,&h);

  // Changes each RGB value for each pixel in canvas
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      canvas[i][j].red = minglitch(canvas[i][j].red);
      canvas[i][j].green = minglitch(canvas[i][j].green);
      canvas[i][j].blue = minglitch(canvas[i][j].blue);
    }
  }

  // Writes glitches to new file
  write_ppm_2d(newfilename,canvas,w,h);

  for (int k = 0; k < h; k++) {
      free(canvas[k]);
  }
  free(canvas);

  return 0;
}

