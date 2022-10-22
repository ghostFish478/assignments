#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// implement one

// struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
//   return NULL;
// }

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  // Holds the value of the contents of the file
  FILE* infile;
  char line[1024];

  // Opens the file
  infile = fopen(filename, "r");
  if (infile == NULL) {
    printf("Unable to open: %s\n", filename);
    return NULL;
  }
  
  // Reads the header
  fgets(line,1024,infile);
  fgets(line,1024,infile);
  // Scans in the width and height of the image from teh file
  fscanf(infile," %d %d", w, h);
  fgets(line,1024,infile);
  // Return NULL if width or height is not found
  // if (w == NULL | h == NULL) {
  //   printf("Unable to allocate memory for %s", filename);
  //   return NULL;
  // }
  fgets(line,1024,infile);

  printf("Reading file %s with width %d and height %d:\n", filename, *w, *h);
  printf("\n");

  // Allocate memory for the array holding the information from the image
  struct ppm_pixel** array = malloc(*h * sizeof(struct ppm_pixel*));
  for (int i = 0; i < *h; i++) {
    array[i] = malloc(*w * sizeof(struct ppm_pixel));
    fread(array[i], sizeof(struct ppm_pixel), *w, infile);
  }
  fclose(infile);
  return array;
}
