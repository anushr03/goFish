#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*
 * Function: shuffle
 * --------------------
 *  Initializes deck_instance and shuffles it.
 *  Resets the deck if a prior game has been played.
 * 
 *  returns: 0 if no error, and non-zero on error
 */ 
int shuffle(){
    //initialize deck
    deck_instance.top_card = 0;
    char suits[4] = {'S', 'H', 'D', 'C'};
    char ranks[13][3] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"};

    //fill deck without using strcpy
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 13; j++){
            deck_instance.list[i*13 + j].suit = suits[i];
            deck_instance.list[i*13 + j].rank[0] = ranks[j][0];
            deck_instance.list[i*13 + j].rank[1] = ranks[j][1];
        }
    }


    //shuffle deck
    srand(time(NULL)); //seed random number generator
    for (int i = 0; i < 52; i++){
        int j = rand() % (i+1); //random number between 0 and i
        struct card temp = deck_instance.list[i]; 
        deck_instance.list[i] = deck_instance.list[j];
        deck_instance.list[j] = temp;
    }
    return 0;


}

/*
 * Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck. 
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */

int deal_player_cards(struct player* target){
    // add random cards from deck to player's hand
    for (int i = 0; i < 7; i++){
        struct card* c = next_card();
        if (c == NULL){
            return 1;
        }
        add_card(target, c);
    }
    return 0;
}

/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck. 
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card( ){
    struct card* top = &deck_instance.list[deck_instance.top_card];
    deck_instance.top_card++;
    return top;
}

size_t deck_size( ){
    return 52 - deck_instance.top_card;
}