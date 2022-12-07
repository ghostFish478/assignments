#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct ppm_pixel newpalette() {
  struct ppm_pixel color;
  color.red = rand() % 255;
  color.green = rand() % 255;
  color.blue = rand() % 255;
  return color;
}

struct ppm_pixel** mandelbrot(int size, int xcol, int ycol, int xrow, int yrow, float xmin, float xmax, float ymin, float ymax, int maxIterations, struct ppm_pixel* palette, struct ppm_pixel** mandelbrotset) {
  for(float i = xrow; i < yrow; i++) {
    for(float j = xcol; j < ycol; j++) {
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
  return mandelbrotset;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

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
  srand(time(0));
  // Timer values
  double timer;
  struct timeval tstart, tend;
  int shmidmandel, shmidbrot, shmidpalette;

  shmidmandel = shmget(IPC_PRIVATE, size * size * sizeof(struct ppm_pixel), 0644 | IPC_CREAT);
  if (shmidmandel == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel* data = shmat(shmidmandel, NULL, 0);
  struct ppm_pixel** mandelbrotset = malloc(size * sizeof(struct ppm_pixel*));

  if (mandelbrotset == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }
  struct ppm_pixel pix;
  pix.red = 255;
  pix.green = 0;
  pix.blue = 0;

  for(int i = 0; i < size; i++) {
    mandelbrotset[i] = &(data[i * size]);
    for(int j = 0; j < size; j++) {
        mandelbrotset[i][j] = pix;
    }
  }

  // Holds value for a new string for the new file name for the outfile
  char newfilename[1000];

  // generate pallet
  struct ppm_pixel* palette = malloc(maxIterations * sizeof(struct ppm_pixel));

/*
  // Gets start time
  gettimeofday(&tstart, NULL);

  for (int k = 0; k < numProcesses; k++) {
    int xcol, ycol, xrow, yrow;
    // float newxmin, newxmax, newymin, newymax;
    // change coordinate
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

    // int pid = fork();

    // if (pid == 0) {
      // compute image
      mandelbrotset = mandelbrot(size, xcol, ycol, xrow, yrow, xmin, xmax, ymin, ymax, maxIterations, palette, mandelbrotset);

    //   for(int z = 0; z < size; z++) {
    //     if (shmdt(mandelbrotset[z]) == -1) {
    //       perror("Error: Mandelbrotset z cannot detach from shared memory\n");
    //       exit(1);
    //     }
    //     if (shmctl(shmidbrot, IPC_RMID, 0) == -1) {
    //       perror("Error: Mandelbrotset z cannot remove shared memory\n");
    //       exit(1);
    //     }
    //   }
    // } else {
    //   printf("Launched child process: %d\n", pid);
    //   printf("%d) Sub-image block: cols (%d, %d) to rows (%d,%d)\n", pid, xcol, ycol, xrow, yrow);
    // }
   }

//   for(int i = 0; i < 4; i++) {
//     int status;
//     int pid = wait(&status);
//     printf("Child process complete: %d\n", pid);
//   }

  // Gets end time
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  time_t timestamp = time(0);

  // Creates the new file name
  snprintf(newfilename, sizeof(newfilename), "nomandelbrot-%d-%ld.ppm", size, timestamp);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  printf("Writing file: %s\n", newfilename);
  */
//   write_ppm_2d(newfilename, mandelbrotset, size, size);
  if(shmdt(data) == -1) {
    perror("Error: Mandelbrotset cannot detach from shared memory\n");
    exit(1);
  }

  if (shmctl(shmidmandel, IPC_RMID, 0) == -1) {
    perror("Error: Mandelbrotset cannot remove shared memory\n");
    exit(1);
  }
  free(palette);
  free(mandelbrotset);
  // compute image
  return 0;
}