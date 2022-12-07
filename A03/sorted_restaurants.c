/***************************************************
 * sorted_restaurants.c
 * Author: Bethany Ho
 * Date: 11/24/2022
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
    char name[32];
    // Holds value for the opening time of restauarant
    int timeOpen;
    // Holds value for closing time of restauarant
    int timeClose;
    // Holds value for rating for restauarant
    double stars;
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
struct restaurantR* insertRest(char* name, int open, int close, double rating, struct restaurantR* head) {
  struct restaurantR* temp = malloc(sizeof(struct restaurantR));
  strcpy(temp->name, name);
  temp->timeOpen = open;
  temp->timeClose = close;
  temp->stars = rating;
  temp->next = NULL;

  // Checks if the new restaurant can be added as the first element of the list
  // Adds if so
  if(head == NULL) {
    head = temp;
    return head;
  }

  // Condition if the new restaurant has a higher rating than the head
  // Assigns the new restaurant as the new head
  if(temp->stars > head->stars) {
    // Head becomes the second object
    temp->next = head;
    head = temp;
    return head;
  }
  // Checks if the new restaurant is less than the first element yet greater than the next elements
  else {
    // Keeps value of the original head
    struct restaurantR* firstHead = head;
    // Moves to the next head if there is still more elements in the list
    // and if the new restaurant has a lower rating than the next element in the list
    while(head->next != NULL && temp->stars < head->next->stars) {
      head = head->next;
    }
    // Inserts the new restaurant into the list
    temp->next = head->next;
    head->next = temp;
    return firstHead;
  }
  return head;
}

/**
 * Prints the entire restaurant list
 * 
 * @param head start of the list
 * @return nothing
 */
void print(struct restaurantR* head) {
  // Holds value for restaurant's number
  int i = 0;
  // Goes through each element in the linked list
  while (head != NULL) {
    printf("%d) %s\topen: %d\tclose: %d:00\tstars: %.02lf\n", i, head->name, head->timeOpen, head->timeClose, head->stars);
    // Changes head to the next element in the list
    head = head->next;
    // Increases the number for the restaurant
    i++;
  }
}

/**
 * Clears the entire restaurant list
 * 
 * @param head start of the list
 * @return nothing
 */
void clearall(struct restaurantR* head) {
  struct restaurantR* temp = NULL;
  // Moves through the list while there are still elements in the list
  // Frees each element
  while(head->next != NULL) {
    temp = head->next;
    free(head);
    head = temp;
  }
  free(head);
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

  // Goes through each restaurant
  for (int i = 0; i < number; i++) {
    // Enter name for restaurant and allocate memory to store name in an object
    printf("Enter a name:");
    scanf(" %[^\n]%*c", restName);

    // Enter and store opening time for restaurant in an object
    printf("Open Time:");
    scanf(" %d", &open);

    // Enter and store closing time for restaurant in an object
    printf("Close Time:");
    scanf(" %d", &close);

    // Enter and store rating for restaurant in an object
    printf("Stars:");
    scanf(" %lf", &rating);

    // Insert new restauarant to linked list
    // Assign new head to head
    head = insertRest(restName, open, close, rating, head);
  }

  // Go through each restaurant and print it out into a table
  // Clear after printing
  print(head);
  clearall(head);

  return 0;
}
