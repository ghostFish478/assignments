/***************************************************
 * sorted_restaurants.c
 * Author: Bethany Ho
 * Date: 10/1/2022
 * Prints restaurant list based on ratings
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for holding restuarant values for name,
// opening time, closing time, and rating, as well as
// next and prev for linked lists
struct restaurantR {
    // Holds value for name of restaurant
    char* name;
    // Holds value for the opening time of restauarant
    int timeOpen;
    // Holds value for closing time of restauarant
    int timeClose;
    // Holds value for rating for restauarant
    double stars;
    // Holds value for individual object
    struct restaurantR* data;
    // Holds value for next restauarant in a list
    struct restaurantR* next;
};

/**
 * Inserts a new element into the restaurant list
 * 
 * @param restNew restaurant to be added to the list
 * @param head starting element of the list
 * @return new head
 */
struct restaurantR* insertRest(struct restaurantR* restNew, struct restaurantR* head) {
  struct restaurantR* temp = malloc(sizeof(struct restaurantR));
  temp = restNew;
  // Checks if the new restaurant can be added as the first element of the list
  // Adds if so
  if (temp->stars > head->stars) {
    temp->next = head;
    head = temp;
  }
  // Checks if the new restaurant is less than the first element yet greater than the next elements
  else if (temp->stars < head->stars && temp->stars > head->next->stars) {
    // Keeps value of the original head
    struct restaurantR* firstHead = head;
    // Moves through the list for the location to insert the new restaurant
    while(temp->stars < head->stars && temp->stars > head->next->stars) {
      head = head->next;
    }
    // Changes the location of next for temp and head
    temp->next = head->next->next;
    head->next = temp;
    // Assigns head to the original head value again
    head = firstHead;
  }
  // Checks if the new restaurant is less than the first and second element of the list
  else if (temp->stars < head->stars && temp->stars < head->next->stars) {
    // Keeps value of the original head
    struct restaurantR* firstHead = head;
    // Moves through the list for the location to insert the new restaurant
    while(temp->stars < head->stars && temp->stars < head->next->stars) {
      head = head->next;
    }
    // Changes the location of next for temp and head
    temp->next = head->next->next;
    head->next = temp;
    // Assigns head to the original head value again
    head = firstHead;
  }
  return head;
}

/**
 * Prints and clears the entire restauarant list
 * 
 * @param head start of the list
 * @return nothing
 */
void clearall(struct restaurantR* head) {
  // Holds value for restaurant's number
  int i = 0;
  // Goes through each element in the linked list
  while (head != NULL) {
    printf("%d) %s\topen: %d\tclose: %d:00\tstars: %.02lf\n", i, head->name, head->timeOpen, head->timeClose, head->stars);
    free(head->name);
    // Changes head to the next element in the list
    head = head->next;
    // Increases the number for the restaurant
    i++;
  }
}

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
  // Holds head of linked list
  struct restaurantR* head = NULL;

  // Scans in number of restaurants
  printf("Enter a number of restaurants:");
  scanf("%d", &number);

  // Allocates memory towards storing information for restaurants
  struct restaurantR* restaurant = malloc(sizeof(struct restaurantR));

  // Goes through each restaurant
  for (int i = 0; i < number; i++) {
    // Enter name for restaurant and allocate memory to store name in an object
    printf("Enter a name:");
    scanf(" %[^\n]%*c", restName);
    restaurant->name = malloc((strlen(restName) + 1) * sizeof(char));
    memset(restaurant->name, 0, strlen(restName) + 1);
    strcpy(restaurant->name, restName);

    // Enter and store opening time for restaurant in an object
    printf("Open Time:");
    scanf(" %d", &open);
    restaurant->timeOpen = open;

    // Enter and store closing time for restaurant in an object
    printf("Close Time:");
    scanf(" %d", &close);
    restaurant->timeClose = close;

    // Enter and store rating for restaurant in an object
    printf("Stars:");
    scanf(" %lf", &rating);
    restaurant->stars = rating;

    // Insert new restauarant to linked list
    // Assign new head to head
    head = insertRest(restaurant, head);
  }

  // Go through each restaurant and print it out into a table
  // Clear after printing
  clearall(head);
  free(head);

  return 0;
}
