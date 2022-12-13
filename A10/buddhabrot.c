#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct data {
  int size, xcol, ycol, xrow, yrow, maxIterations;
  float xmin, xmax, ymin, ymax;
  int id;
  int maxCount;
  int** buddhabrotcount;
  struct ppm_pixel** buddhabrot;
};

struct ppm_pixel** start(void* data) {
  pthread_t *threads;
  struct data *buddhathread;

  pthread_barrier_t barrier;
  threads = malloc(4 * sizeof(pthread_t));
  buddhathread = malloc(4 * sizeof(struct data));
  // struct data *buddhathread = (struct data *) data;

  for(int a = 0; a < 4; a++) {
    buddhathread[a] = (struct data*)data;
    buddhathread[a].id = a;
    if(a == 0) {
      buddhathread[a].xcol = 0;
      buddhathread[a].ycol = buddhathread[a].size/2;
      buddhathread[a].xrow = 0;
      buddhathread[a].yrow = buddhathread[a].size/2;
    } else if (a == 1) {
      buddhathread[a].xcol = buddhathread[a].size/2;
      buddhathread[a].ycol = buddhathread[a].size;
      buddhathread[a].xrow = 0;
      buddhathread[a].yrow = buddhathread[a].size/2;
    } else if (a == 2) {
      buddhathread[a].xcol = 0;
      buddhathread[a].ycol = buddhathread[a].size/2;
      buddhathread[a].xrow = buddhathread[a].size/2;
      buddhathread[a].yrow = buddhathread[a].size;
    } else if (a == 3) {
      buddhathread[a].xcol = buddhathread[a].size/2;
      buddhathread[a].ycol = buddhathread[a].size;
      buddhathread[a].xrow = buddhathread[a].size/2;
      buddhathread[a].yrow = buddhathread[a].size;
    }
  }

  for(int i = 0; i < 4; i++) {
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n", buddhathread[i].id, buddhathread[i].xcol, buddhathread[i].ycol, buddhathread[i].xrow, buddhathread[i].yrow);
    // perform step 1
    pthread_create(&threads[i], NULL, membership, &buddhathread[i]);
    // perform step 2
    pthread_create(&threads[i], NULL, visitedCounts, &buddhathread[i]);
  }
  // use a thread barrier to wait for all threads to finish steps 1 and 2
  pthread_barrier_wait (&barrier);
  // perform step 3
  for (int j = 0; j < 4; j++) {
    pthread_create(&threads[j], NULL, palette, &buddhathread[j]);
  }
  for (int k = 0; k < 4; k++) {
    pthread_join(threads[k], NULL);
    printf("Thread %d) finished", buddhathread[k].id);
  }
  return buddhathread->buddhabrot;
}

// int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, struct ppm_pixel** buddhabrot
void* membership(void* data) {
  struct data *buddhamem = (struct data *) data;
  //  for each row in the image
  for(int i = 0; i < buddhamem->size; i++) {
    //  for each col in the image
    for(int j = 0; j < buddhamem->size; j++) {
      float xfrac = i / buddhamem->size;
      float yfrac = j / buddhamem->size;
      float x0 = buddhamem->xmin + xfrac * (buddhamem->xmax - buddhamem->xmin);
      float y0 = buddhamem->ymin + yfrac * (buddhamem->ymax - buddhamem->ymin);

      int x = 0;
      int y = 0;
      int iter = 0;

      while (iter < buddhamem->maxIterations && x*x + y*y < 4) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      
      if (iter < buddhamem->maxIterations) {// escaped
        // set membership at (row,col) to false
        buddhamem->buddhabrotcount[i][j] = 0;
      } else {
        // set membership at (row,col) to true
        buddhamem->buddhabrotcount[i][j] = 1;
      }
    }
  }
  return (void*)NULL;
}

void* visitedCounts(void* data) {
  struct data *buddhamem = (struct data *) data;
  // for each row in the image
  for(int i = 0; i < buddhamem->size; i++) {
    // for each col in the image
    for(int j = 0; j < buddhamem->size; j++) {
      // if (row,col) belongs to the mandelbrot set, continue
      if(buddhamem->buddhabrotcount[j][i] == 1) {
        buddhamem->buddhabrotcount[j][i] = 0;
        continue;
      } else {
        break;
      }

      float xfrac = i / buddhamem->size;
      float yfrac = j / buddhamem->size;
      float x0 = buddhamem->xmin + xfrac * (buddhamem->xmax - buddhamem->xmin);
      float y0 = buddhamem->ymin + yfrac * (buddhamem->ymax - buddhamem->ymin);

      int x = 0;
      int y = 0;
      while (x*x + y*y < 4) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        
        buddhamem->yrow = round(buddhamem->size * (y - buddhamem->ymin)/(buddhamem->ymax - buddhamem->ymin));
        buddhamem->xcol = round(buddhamem->size * (x - buddhamem->xmin)/(buddhamem->xmax - buddhamem->xmin));
        if (buddhamem->yrow < 0 || buddhamem->yrow >= buddhamem->size) {
          continue;
        } // out of range
        if (buddhamem->xcol < 0 || buddhamem->xcol >= buddhamem->size) {
          continue; // out of range
        }
        // increment count at (yrow, xcol)
        buddhamem->buddhabrotcount[buddhamem->yrow][buddhamem->xcol] += 1;
        // update max count
        buddhamem->maxCount = buddhamem->maxCount + 1;
      }
    }
  }
  return (void*)NULL;
}

void* palette(void* data) {
  struct data *buddhamem = (struct data *) data;
  float gamma = 0.681;
  float factor = 1.0/gamma;
  // for each row in the image
  for(int i = 0; i < buddhamem->size; i++) {
    // for each col in the image
    for(int j = 0; j < buddhamem->size; j++) {
      int value = 0;

      if(buddhamem->buddhabrotcount[j][i] > 0) {
        value = log(buddhamem->buddhabrotcount[j][i]) / log(buddhamem->maxCount);
        value = pow(value, factor);
      }
      struct ppm_pixel color;
      color.red = value * 255;
      color.green = value * 255;
      color.blue = value * 255;

      buddhamem->buddhabrot[i][j] = color;
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

  // Gets start time
  gettimeofday(&tstart, NULL);

  // Creates canvas for buddhabrot
  struct ppm_pixel** buddhabrot = malloc(size * sizeof(struct ppm_pixel*));
  for(int j = 0; j < size; j++) {
    buddhabrot[j] = malloc(size * sizeof(struct ppm_pixel));
  }
  
  // Creates tracker for buddhabrot counts
  int** buddhabrotcount = malloc(size * sizeof(int*));
  for(int i = 0; i < size; i++) {
    buddhabrotcount[i] = malloc(size * sizeof(int));
  }

  // Holds structure for data
  struct data data;

  // Initializes values
  // Holds value for size
  data.size = size;
  // Holds values for locations on a canvas
  data.xcol = 0;
  data.ycol = 0;
  data.xrow = 0;
  data.yrow = 0;
  // Holds number of iterations
  data.maxIterations = maxIterations;

  // Holds values for buddhabrot calculations
  data.xmin = xmin;
  data.xmax = xmax;
  data.ymin = ymin;
  data.ymax = ymax;

  // Holds id value
  data.id = 0;
  // Holds max count
  data.maxCount = 0;
  // Holds buddhabrot count tracker
  data.buddhabrotcount = buddhabrotcount;
  // Holds buddhabrot canvas
  data.buddhabrot = buddhabrot;

  // compute image
  *buddhabrot = start((void*)data);

  // Gets end time
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  time_t timestamp = time(0);

  // Creates the new file name
  // Holds value for a new string for the new file name for the outfile
  char newfilename[1000 * sizeof(char)];
  snprintf(newfilename, sizeof(newfilename), "buddhabrot-%d-%ld.ppm", size, timestamp);
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);

  // Writes the new file
  printf("Writing file: %s\n", newfilename);
  write_ppm_2d(newfilename, buddhabrot, size, size);
  free(buddhabrot);
  free(buddhabrotcount);
}
