/***************************************************
 * power.c
 * Author: Bethany Ho
 * Date: 9/13/2022
 * Makes bad password
 */

#include <stdio.h>
#include <string.h>

int power(int base, int exp) {
    int exponent = 0;
    for(int i =1; i < exp; i++) {
        exponent = base*base;
    }
    return exponent;
}

int main() {
    int base = 3;
    int exp = 4;
    int result = power(base, exp);
    printf("%d", result);
}