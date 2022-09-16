/***************************************************
 * wordle.c
 * Author: Bethany Ho
 * Date: 9/15/2022
 * Makes wordle
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "words.h"

/**
 * Checks if each given letter matches each letter in
 * the Wordle.
 * 
 * @param chosenWord word chosen for the Wordle
 * @param guessWord word used the guess the Wordle
 * @return print statement, 0 when finished
 */
int guessLoop(const char* chosenWord, char* guessWord) {
    // Counter for tracking letters that are not in the right place
    int wrongCounter = 0;
    // Counter for tracking letters that are in the right place
    int rightCounter = 0;

    // Counter for checking for correct letters
    for(int i = 0; i < 5; i++){
        // Checks if the letter placement matches
        if (chosenWord[i] == guessWord[i]) {
            printf("%c is in the word AND in the correct spot!\n", guessWord[i]);
            // Increase counter for right letter placement
            rightCounter++;
        }
        else if (chosenWord[i] != guessWord[i]) {
            for(int j = 0; j < 5; j++) {
                if (chosenWord[j] != guessWord[i]){
                    // Increase counter for wrong letter placement
                    wrongCounter++;
                }
            }

            // Prints statement for if the letter does not exist in the word
            if (wrongCounter == 5) {
                printf("%c is NOT in the word.\n", guessWord[i]);
                wrongCounter = 0;
            } else {
                printf("%c is in the word (but not in the correct spot)!\n", guessWord[i]);
                wrongCounter = 0;
            }
        }

        // Returns 1 if all letters in the guessed word are correct
        if (rightCounter == 5) {
            printf("Congratulations!");
            return 1;
        }
        
    }
    return 0;
}

int main() {
    srand(time(0));
    // Random 5-letter word
    const char* word = chooseWord();
    // Initializes array for guess
    char guess[128];
    
    printf("Welcome to Wordle!\n");

    // Loops for six guesses
    for(int x = 0; x < 6; x++) {
        printf("Please enter a 5-letter world:\n");
        // Assigns word used for guessing
        scanf(" %s", guess);
        
        // Function of guessLoop checks if all letters are correct with the chosen word
        int guessCorrectly = guessLoop(word, guess);
        // Loop breaks if all letters are indeed correct
        if (guessCorrectly == 1) {
            break;
        }

        // Prints correct answer
        if (x == 5) {
            printf("%s is the correct answer. Try again next time!", word);
        }
    }

  return 0;
}