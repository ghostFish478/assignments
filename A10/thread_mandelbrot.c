#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

// Generates a palette color
struct ppm_pixel newpalette() {
  struct ppm_pixel color;
  color.red = rand() % 255;
  color.green = rand() % 255;
  color.blue = rand() % 255;
  return color;
}

// Holds variables for mandelbrot
struct data {
  // Holds variables for size, location, iterations, id, and boundaries for mandelbrot
  int size, xcol, ycol, xrow, yrow, maxIterations, id;
  float xmin, xmax, ymin, ymax;
  // Holds values for palettes
  struct ppm_pixel* palette;
  // Holds value for canvas
  struct ppm_pixel** mandelbrotset;
};

// Runs mandelbrot algorithm and allocates colors
void *mandelbrot(void* data) {
  struct data *mandelthread = (struct data *) data;
  for(float i = 0; i < mandelthread->size; i++) {
    for(float j = 0; j < mandelthread->size; j++) {
      float xfrac = i/mandelthread->size;
      float yfrac = j/mandelthread->size;
      float x0 = mandelthread->xmin + xfrac * (mandelthread->xmax - mandelthread->xmin);
      float y0 = mandelthread->ymin + yfrac * (mandelthread->ymax - mandelthread->ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < mandelthread->maxIterations && x*x + y*y < 4) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      // write color to image at location (row,col)
      if (iter < mandelthread->maxIterations) { // escaped
        mandelthread->mandelbrotset[(int)j][(int)i] = mandelthread->palette[iter];
      } else {
        mandelthread->mandelbrotset[(int)j][(int)i].red = 0;
        mandelthread->mandelbrotset[(int)j][(int)i].green = 0;
        mandelthread->mandelbrotset[(int)j][(int)i].blue = 0;
      }
    }
  }
  return (void*)NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  srand(time(0));

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
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
  for(int j = 0; j < maxIterations; j++) {
    palette[j] = newpalette();
  }

  // Holds value for coordinates
  int xcol, ycol, xrow, yrow;
  // Holds value for threads
  pthread_t *threads;
  // Holds value for structure for threads
  struct data *mandelthread;
  // Gets start time
  gettimeofday(&tstart, NULL);

  // Creates array for threads
  threads = malloc(4 * sizeof(pthread_t));
  // Creates array for mandelthread
  mandelthread = malloc(4 * sizeof(struct data));

  // compute image
  for (int k = 0; k < 4; k++) {
    // Initializes variables for mandelthread
    mandelthread[k].size = size;
    mandelthread[k].xcol = xcol;
    mandelthread[k].ycol = ycol;
    mandelthread[k].xrow = xrow;
    mandelthread[k].yrow = yrow;
    mandelthread[k].maxIterations = maxIterations;
    mandelthread[k].id = k;
    mandelthread[k].xmin = xmin;
    mandelthread[k].xmax = xmax;
    mandelthread[k].ymin = ymin;
    mandelthread[k].ymax = ymax;
    mandelthread[k].palette = palette;
    mandelthread[k].mandelbrotset = mandelbrotset;
    // change coordinates
    if(k == 0) {
      xcol = 0;
      ycol = size/2;
      xrow = 0;
      yrow = size/2;
    } else if (k == 1) {
      xcol = size/2;
      ycol = size;
      xrow = 0;
      yrow = size/2;
    } else if (k == 2) {
      xcol = 0;
      ycol = size/2;
      xrow = size/2;
      yrow = size;
    } else if (k == 3) {
      xcol = size/2;
      ycol = size;
      xrow = size/2;
      yrow = size;
    }
    // Creates threads
    pthread_create(&threads[k], NULL, mandelbrot, &mandelthread[k]);
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n", mandelthread[k].id, xcol, ycol, xrow, yrow);
  }

  // Joins threads
  for (int l = 0; l < 4; l++) {
    pthread_join(threads[l], NULL); 
    printf("Thread %d) finished", mandelthread[l].id);
  }

  // Gets end time
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  time_t timestamp = time(0);

  // Creates the new file name
  snprintf(newfilename, sizeof(newfilename), "mandelbrot-%d-%ld.ppm", size, timestamp);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // Writes the new file
  printf("Writing file: %s\n", newfilename);
  write_ppm_2d(newfilename,  mandelthread->mandelbrotset, size, size);

  // Frees memory
  for(int a = 0; a < size; a++) {
    free(mandelbrotset[a]);
  }
  free(mandelbrotset);
  free(palette);
  free(threads);
  free(mandelthread);
  return 0;
}