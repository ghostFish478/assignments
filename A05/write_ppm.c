#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
// }

// write in a PPM file in binary format
// filename: the file to save to
// pxs: a 2D array of ppm_pixel to save
// w: the width of the image
// h: the height of the image
void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
  // Holds the value of the contents for the outfile
  FILE* outfile;

  // Opens the outfile
  outfile = fopen(filename, "wb");

  // Prints the header
  char* line = "P6\n# Created by GIMP version 2.10.24 PNM plug-in\n";
  fprintf(outfile, "%s", line);
  fprintf(outfile, "%d %d", w, h);
  fprintf(outfile,"%s", "\n255\n");

  printf("Writing file %s\n", filename);
  printf("\n");

  // Writes to the new file
  for (int i = 0; i < h; i++) {
    fwrite(pixels[i], sizeof(struct ppm_pixel), w, outfile);
  }
  fclose(outfile);
}