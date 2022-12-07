#include <stdio.h>

int main() {
  // Holds value for image
  unsigned long img;
  // Scans image value
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // Holds value for the left mask covering bits
  unsigned long leftmask;
  // Holds value for the loop determining the number of masked bits
  unsigned long loop = 63;
  // Holds value for the new masked bits; initializes value for masked img
  unsigned long maskedimg = img;

  // Loops thorough 8x8 matrix for printing values
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      // Creates left mask depending on how many bits are masked through loop
      leftmask = 0x1ul << 63;
      maskedimg = img << loop;
      // Creates comparison to see what the masked bit is
      maskedimg = maskedimg & leftmask;
      // Determines the value of the output depending on the bit's value
      if(maskedimg == 0) {
        printf(" ");
      }
      else {
        printf("@");
      }
      // Decreases value of loop to create new mask
      loop = loop-1;
    }
    // New line for creating 8x8 matrix
    printf("\n");
  }

  printf("%lx", img);

  return 0;
}
