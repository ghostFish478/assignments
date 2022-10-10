/***************************************************
 * read_ppm.c
 * Author: Bethany Ho
 * Date: 10/9/2022
 * Contains function for reading a PPM file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement
// struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
//   return NULL;
// }

/**
 * Reads in a PPM file image and returns a ppm array
 * 
 * @param filename the name of the file
 * @param w width of the image
 * @param h height of the image
 * @return array
 */
struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  // Holds the value of the contents of the file
  FILE* infile;

  // Opens the file
  infile = fopen(filename, "r");
  if (infile == NULL) {
    printf("Unable to open: %s", filename);
    return NULL;
  }
  
  // Reads the header
  fgets(infile);
  fgets(infile);
  // Scans in the width and height of the image from teh file
  fscanf("%d %d", &w, &h);
  // Return NULL if width or height is not found
  if (w == NULL | h == NULL) {
    printf("Unable to allocate memory for %s", filename);
    return NULL;
  }
  // printf("Testing file %s %d %d:", filename, w, h);
  printf("Reading file %s %d %d:", filename, w, h);

  // Allocate memory for the array holding the information from the image
  struct ppm_pixel* array = malloc(w * sizeof(struct ppm_pixel*));
  for (int i = 0; i < w; i++) {
    struct ppm_pixel* array[i] = malloc(h * sizeof(struct ppm_pixel*));
  }

  // Read the pixels as binary
  for(int j = 0; j < w; j++) {
    for(int k = 0; k < h; k++) {
      fread(array[j][k].red, 1, 1, infile);
      fread(array[j][k].green, 1, 1, infile);
      fread(array[j][k].blue, 1, 1, infile);
    }
  }
  return array;
}