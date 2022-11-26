/***************************************************
 * test_ppm.c
 * Author: Bethany Ho
 * Date: 10/9/2022
 * Reads and prints the RGB values for a PPM file
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  const char* filename = "feep-raw.ppm";
  // load feep-raw and print out the grid of pixels
  struct ppm_pixel** ppmarray = read_ppm_2d(filename, &w, &h);
    for(int i = 0; i < h; i++) {
      for(int j = 0; j < w; j++) {
        printf("(%u, %u, %u)", ppmarray[i][j].red, ppmarray[i][j].green, ppmarray[i][j].blue);
      }
      printf("\n");
    }

    for (int k = 0; k < h; k++) {
      free(ppmarray[k]);
    }
    free(ppmarray);
  return 0;
}