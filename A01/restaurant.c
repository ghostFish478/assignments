/***************************************************
 * restaurant.c
 * Author: Bethany Ho
 * Date: 9/15/2022
 * Makes list of restaurants and outputs feedback based on time
 */

#include <stdio.h>
#include <string.h>

// Structure for holding restuarant values for name,
// opening time, closing time, and rating
struct restaurantR {
    // Holds value for name of restaurant
    char name[128];
    // Holds value for the opening time of restauarant
    int timeOpen;
    // Holds value for closing time of restauarant
    int timeClose;
    // Holds value for rating for restauarant
    double stars;
};

/**
 * Checks the time and compares it with the opening
 * and closing time to see if a given restaurant is
 * open.
 * 
 * @param currentTime chosen time
 * @param wantedR chosen restaurant
 * @return print statement, 0 when finished
 */
int checkTime(int currentTime, int wantedR) {
    struct restaurantR rest1, rest2, rest3;
    strcpy(rest1.name, "Sushi Atsumi");
    rest1.timeOpen = 11;
    rest1.timeClose =23;
    rest1.stars = 3.5;

    strcpy(rest2.name, "The Thirsty Horse");
    rest2.timeOpen = 17;
    rest2.timeClose = 2;
    rest2.stars = 4.5;

    strcpy(rest3.name, "Magic Bean Coffee");
    rest3.timeOpen = 6;
    rest3.timeClose = 15;
    rest3.stars = 4.1;

    // Array used for helping to retrieve information for a given restaurant
    struct restaurantR restaurant[3];
    restaurant[0] = rest1;
    restaurant[1] = rest2;
    restaurant[2] = rest3;

    // Integer for holding value of how much time is left for an open restaurant
    int timeLeft;

    // Checks how much time is left if the current time is while a restaurant is open
    if ((restaurant[wantedR].timeOpen <= currentTime) || (0 < currentTime && currentTime < restaurant[wantedR].timeClose)) {
        timeLeft = restaurant[wantedR].timeClose - currentTime;
        // Corrects timeLeft if it is a negative value
        if (timeLeft < 0) {
            timeLeft = timeLeft + 24;
        }
        printf("Excellent choice. %s will be open for %d more hours.\n", restaurant[wantedR].name, timeLeft);

    } else {
        // Checks how close to the opening time the current time is
        int closeToOpen = restaurant[wantedR].timeOpen - currentTime;
        // Corrects closeToOpen if it is a negative value
        if (closeToOpen < 0) {
            closeToOpen = closeToOpen * -1;
        }
        // Checks how close to the closing time the current time is
        int closeToClose = currentTime - restaurant[wantedR].timeClose;
        // Corrects closeToClose if it is a negative value
        if (closeToClose < 0) {
            closeToClose = closeToClose * -1;
        }

        // Compares whether the time interval after closing and before opening is larger
        // for print statement
        if (closeToOpen > closeToClose) {
            printf("%s closed at %d o'clock!!", restaurant[wantedR].name, restaurant[wantedR].timeClose);
        } else if (closeToClose > closeToOpen) {
            printf("%s isn't open until %d o'clock!!", restaurant[wantedR].name, restaurant[wantedR].timeOpen);
        } else if (closeToClose == closeToOpen) {
            printf("%s isn't open until %d o'clock!!", restaurant[wantedR].name, restaurant[wantedR].timeOpen);
        }
    }
    return 0;
}

int main() {
    // Integer value for current time
    int timeNow;
    // Integer value to represent chosen restaurant
    int visitNow;

    printf("Welcome to Bethany Ho's Restarant List.\n");

    printf("What hour is it (24 hr clock)?\n");
    scanf("%d", &timeNow);

    printf("What restaurant do you want to visit? [0,1,2]\n");
    scanf("%d", &visitNow);

    // Gives feedback based on time and restaurant given
    checkTime(timeNow, visitNow);
    return 0;
}