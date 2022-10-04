#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct flavor {
  float calories;
  char name[32];
};

struct cupcake {
  struct flavor* cake;
  struct flavor* frosting;
};

struct flavor* makeFlavor(const char* name, float cals) {
  struct flavor* c = malloc(sizeof(struct flavor));
  if (c == NULL) {
    printf("ERROR: Out of memory!\n");
    exit(1);
  }

  strcpy(c->name, name);
  c->calories = cals;
  return c;
}

void computeCalories(struct cupcake* cakes, int size) {
  float max_calories = 0.0;
  struct cupcake most_calories;

  for (int i = 0; i < size; i ++) {
    float total = cakes[i].cake->calories + cakes[i].frosting->calories;
    if (total > max_calories) {
      max_calories = total;
      most_calories = cakes[i];
    }
    printf("Cupcake %d) %s with %s (calories: %f)\n", i, cakes[i].cake->name, cakes[i].frosting->name, total);
  }
  printf("The cup cake with the most calories is %s cake with %s frosting\n", most_calories.cake->name, most_calories.frosting->name);
}

int main() {
  struct flavor* redVelvet = makeFlavor("red velvet", 200);
  struct flavor* chocolate = makeFlavor("chocolate", 175);
  struct flavor* strawberry = makeFlavor("strawberry", 100);
  struct flavor* creamcheese = makeFlavor("cream cheese", 400);
  
  struct cupcake cakes[3];
  cakes[0].cake = redVelvet;
  cakes[0].frosting = creamcheese; 
  cakes[1].cake = strawberry;
  cakes[1].frosting = chocolate; 
  cakes[2].cake = creamcheese;
  cakes[2].frosting = strawberry; 

  computeCalories(cakes, 3);
  free(redVelvet);
  free(chocolate);
  free(strawberry);
  free(creamcheese);
}
