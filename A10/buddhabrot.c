#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct data {
  int size, xcol, ycol, xrow, yrow, maxIterations, id;
  float xmin, xmax, ymin, ymax;
  int** buddhabrotcount;
  struct ppm_pixel** buddhabrot;
}

void *start(void* data) {
  pthread_barrier_t barrier;
  threads = malloc(4 * sizeof(pthread_t));
  buddhathread = malloc(4 * sizeof(struct data));
  struct data *buddhathread = (struct data *) data;
  // buddhathread = malloc(size * size * sizeof(int));

  for(int a = 0; a < 4; a++) {
    buddhathread[k].size = size;
    buddhathread[k].xcol = xcol;
    buddhathread[k].ycol = ycol;
    buddhathread[k].xrow = xrow;
    buddhathread[k].yrow = yrow;
    buddhathread[k].maxIterations = maxIterations;
    buddhathread[k].id = k;
    buddhathread[k].xmin = xmin;
    buddhathread[k].xmax = xmax;
    buddhathread[k].ymin = ymin;
    buddhathread[k].ymax = ymax;
    buddhathread[k].buddhabrot = buddhabrot;
  }

  for(int i = 0; i < 4; i++) {
    // perform step 1
    pthread_create(&threads[i], NULL, membership, &buddhathread[i]);
    // perform step 2
    pthread_create(&threads[i], NULL, visitedCounts, &buddhathread[i]);
  }
  // use a thread barrier to wait for all threads to finish steps 1 and 2
  pthread_barrier_wait (&barrier);
  // perform step 3
  for (j = 0; j < 4; j++) {
    pthread_create(&threads[j], NULL, palette, &buddhathread[j]);
  }
  for (k = 0; k < 4; k++) {
    pthread_join(threads[k], NULL);
  }
  return buddhathread->buddhabrot;
}

// int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, struct ppm_pixel** buddhabrot
void* membership(void* data) {
  struct data *mandelthread = (struct data *) buddhamem;
  //  for each row in the image
  for(int i = 0; i < buddhamem->size; i++) {
    //  for each col in the image
    for(int j = 0; j < buddhamem->size; j++) {
      float xfrac = row / buddhamem->size;
      float yfrac = col / buddhamem->size;
      float x0 = xmin + xfrac * (buddhamem->xmax - buddhamem->xmin);
      float y0 = ymin + yfrac * (buddhamem->ymax - buddhamem->ymin);

      x = 0;
      y = 0;
      int iter = 0;

      while (iter < maxIterations && x*x + y*y < 4) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      
      if (iter < maxIterations) {// escaped
      // set membership at (row,col) to false
        buddhabrotcount[j][i] = 0;
      } else {
      // set membership at (row,col) to true
        buddhabrotcount[j][i] = 1;
      }
    }
  }
  return (void*)NULL;
}

void* visitedCounts(void* data) {
  struct data *mandelthread = (struct data *) buddhamem;
  int maxCount;
  // for each row in the image
  for(int i = 0; i < size; i++) {
    // for each col in the image
    for(int j = 0; j < size; j++) {
      // if (row,col) belongs to the mandelbrot set, continue
      if(buddhabrotcount[j][i] == 1) {
        buddhabrotcount[j][i] = 0;
        continue;
      } else {
        break;
      }

      float xfrac = row / size
      float yfrac = col / size
      float x0 = xmin + xfrac * (xmax - xmin)
      float y0 = ymin + yfrac * (ymax - ymin)

      int x = 0;
      int y = 0;
      while (x*x + y*y < 4) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        
        float yrow = round(size * (y - ymin)/(ymax - ymin))
        float xcol = round(size * (x - xmin)/(xmax - xmin))
        if (yrow < 0 || yrow >= size) continue; // out of range
        if (xcol < 0 || xcol >= size) continue; // out of range

        // increment count at (yrow, xcol)
        buddhabrotcount[j][i] += 1;
        // update max count
        maxCount++;
      }
    }
  }
  return (void*)NULL;
}

void* palette(void* data) {
  struct data *mandelthread = (struct data *) buddhamem;
  float gamma = 0.681
  float factor = 1.0/gamma
  // for each row in the image
  for(int i = 0; i < buddhamem->size; i++) {
    // for each col in the image
    for(int j = 0; j < buddhamem->size; j++) {
      int value = 0;

      if(buddhamem->buddhatbrotcounts[j][i] > 0) {
        value = log(counts[row][col]) / log(max_count);
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
  buddhabrot = start(data);
  // compute image
  write_ppm_2d(filename, size, size, buddhabrot)
}
