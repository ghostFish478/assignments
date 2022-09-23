/***************************************************
 * dynamic_restaurants.c
 * Author: Bethany Ho
 * Date: 9/22/2022
 * Gives a stored list of restaurants
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for holding restuarant values for name,
// opening time, closing time, and rating
struct restaurantR {
    // Holds value for name of restaurant
    char* name;
    // Holds value for the opening time of restauarant
    int timeOpen;
    // Holds value for closing time of restauarant
    int timeClose;
    // Holds value for rating for restauarant
    double stars;
};

int main() {
  // Holds number of restaurants
  int number;
  // Holds name of a restaurant
  char restName[32];
  // Holds restaurant's opening time
  int open;
  // Holds restaurant's closing time
  int close;
  // Holds restaurant's rating
  double rating;

  // Scans in number of restaurants
  printf("Enter a number of restaurants:");
  scanf("%d", &number);

  // Allocates memory towards storing information for restaurants
  struct restaurantR* restaurant = malloc(number * sizeof(struct restaurantR));

  // Goes through each restaurant
  for (int i = 0; i < number; i++) {
    // Enter name for restaurant and allocate memory to store name in an object
    printf("Enter a name:");
    scanf(" %[^\n]%*c", restName);
    restaurant[i].name = malloc((strlen(restName) + 1) * sizeof(char));
    memset(restaurant[i].name, 0, strlen(restName) + 1);
    strcpy(restaurant[i].name, restName);

    // Enter and store opening time for restaurant in an object
    printf("Open Time:");
    scanf(" %d", &open);
    restaurant[i].timeOpen = open;

    // Enter and store closing time for restaurant in an object
    printf("Close Time:");
    scanf(" %d", &close);
    restaurant[i].timeClose = close;

    // Enter and store rating for restaurant in an object
    printf("Stars:");
    scanf(" %lf", &rating);
    restaurant[i].stars = rating;
  }

  // Go through each restaurant and print it out into a table
  for (int i = 0; i < number; i++) {
    printf("%d) %s\topen: %d\tclose: %d:00\tstars: %.02lf\n", i, restaurant[i].name, restaurant[i].timeOpen, restaurant[i].timeClose, restaurant[i].stars);
    free(restaurant[i].name);
  }

  free(restaurant);

  return 0;
}