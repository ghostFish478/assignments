#include <stdio.h>

int main() {
  float a1, a2, a3, b1, b2, b3;
  float epsilon = 0.00001;

  printf("Please enter the values for the first vector:\n");
  scanf("%f %f %f", &a1, &a2, &a3);
  printf("Please enter the values for the second vector\n");
  scanf("%f %f %f", &b1, &b2, &b3);

  float dotproduct = (a1 * b1) + (a2 * b2) + (a3 * b3);
  if (dotproduct > -(epsilon) && dotproduct < epsilon) {
    printf("The dot product is 0.00!\n");
    printf("The vectors are perpendicular!");
  } else {
    printf("The dot product is %f!\n", dotproduct);
    printf("The vectors are NOT perpendicular!\n");
  }
    

  return 0;
}
