#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randomPrice() { //gets random number between 3000 and 5000
  int num = (rand() % (2001)) + 3000;
  return num;
}

int main() {
  int yourGuess;
  srand(time(0));

  printf("Welcome to the Price is right!\n");
  printf("Enter a guess to win a luxurious dishwasher! "); 
  scanf(" %d", &yourGuess); //your guess is scanned in

  //get random price and random guesses
  int dishwasherPrice = randomPrice(); 
  int guess1 = randomPrice();
  int guess2 = randomPrice();

  //find difference between price and guess
  int yourdiff = dishwasherPrice - yourGuess; 
  int AI1diff = dishwasherPrice - guess1;
  int AI2diff = dishwasherPrice - guess2;

  //if diff was negative, aka guess was greater, set diff very high
  if (yourdiff < 0) {
    yourdiff = 5000;
  }
  if (AI1diff < 0) {
    AI1diff = 5000;
  }
  if (AI2diff < 0) {
    AI2diff = 5000;
  }

  //print statements
  printf("AI contestant #1 guesses %d\n", guess1);
  printf("AI contestant #2 guesses %d\n", guess2);
  printf("The dishwasher cost %d\n", dishwasherPrice);

  //if a difference is smaller than the other two, that contestant wins
  //else, no one wins
  if ((yourdiff < AI1diff) && (yourdiff < AI2diff)) {
    printf("Congratulations!! You win the dishwasher!\n");
  } else if ((AI1diff < yourdiff) && (AI1diff < AI2diff)) {
    printf("AI contestant #1 wins the dishwasher!\n");
  } else if ((AI2diff < yourdiff) && (AI2diff < AI1diff)) {
    printf("AI contestant #2 wins the dishwasher!\n");
  } else {
    printf("Sorry. No one wins the dishwasher. I'm keeping it.\n");
  }

  return 0;
}

