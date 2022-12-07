/***************************************************
 * palindrome.c
 * Author: Bethany Ho
 * Date: 9/13/2022
 * Checks for palindrome
 */

#include <stdio.h>
#include <string.h>

int isPalindrome(char* possiblePalin) {
    int length = sizeof(possiblePalin);
    int counter = 0;

    for(int i = 0; i < length; i++) {
        if (possiblePalin[i] != possiblePalin[length - 1 - i]) {
            return 0;
        }
        counter++;
    }
    
    if(counter > 0) {
        return 1;
    }

    return 0;
}

int main() {
    char possiblePalin[128];

    printf("Enter word:");
    scanf("%c", possiblePalin);
    int yesOrNo = isPalindrome(possiblePalin);
    if (yesOrNo == 1) {
        printf("Yes, it is a palindrome.");
    } else {
        printf("No, this is not a palindrome.");
    }
}