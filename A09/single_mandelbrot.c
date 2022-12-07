#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

// Generates a palette color
struct ppm_pixel newpalette() {
  struct ppm_pixel color;
  color.red = rand() % 255;
  color.green = rand() % 255;
  color.blue = rand() % 255;
  return color;
}

int main(int argc, char* argv[]) {
  int size = 2000;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  srand(time(0));

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  // Timer values
  double timer;
  struct timeval tstart, tend;

  // Create canvas for mandelbrotset
  struct ppm_pixel** mandelbrotset = malloc(size * sizeof(struct ppm_pixel*));
  for (int i = 0; i < size; i++) {
    mandelbrotset[i] = malloc(size * sizeof(struct ppm_pixel));
  }

  // Holds value for a new string for the new file name for the outfile
  char newfilename[1000 * sizeof(char)];

  // Generate palette
  struct ppm_pixel* palette= malloc(maxIterations * sizeof(struct ppm_pixel));
  for(int k = 0; k < maxIterations; k++) {
    palette[k] = newpalette();
  }

  // Gets start time
  gettimeofday(&tstart, NULL);

  // compute image
  for(float i = 0; i < size; i++) {
    for(float j = 0; j < size; j++) {
      float xfrac = i/size;
      float yfrac = j/size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < maxIterations && x*x + y*y < 4) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      // write color to image at location (row,col)
      if (iter < maxIterations) { // escaped
        mandelbrotset[(int)j][(int)i] = palette[iter];
      } else {
        mandelbrotset[(int)j][(int)i].red = 0;
        mandelbrotset[(int)j][(int)i].green = 0;
        mandelbrotset[(int)j][(int)i].blue = 0;
      }
    }
  }

  // Gets end time
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  time_t timestamp = time(0);

  // Creates the new file name
  snprintf(newfilename, sizeof(newfilename), "mandelbrot-%d-%ld.ppm", size, timestamp);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  printf("Writing file: %s\n", newfilename);
  write_ppm_2d(newfilename, mandelbrotset, size, size);

  // Frees memory
  for(int a = 0; a < size; a++) {
      free(mandelbrotset[a]);
  }
  free(mandelbrotset);
  free(palette);
  return 0;
}