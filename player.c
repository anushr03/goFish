#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "player.h"
#include "card.h"

/*
 * Function: add_card
 * -------------------
 *  Add a new card to the player's hand. 
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card){

    struct hand* new_hand = malloc(sizeof(struct hand));
    if (new_hand == NULL){
        return -1;
    }

    //add card to the end of the list
    new_hand->top = *new_card;
    new_hand->next = NULL;
    if (target->card_list == NULL){
        target->card_list = new_hand;
    } else {
        struct hand* current = target->card_list;
        while (current->next != NULL){
            current = current->next;
        }
        current->next = new_hand;
    }
    target->hand_size++;
    return 0;
 
}

/*
 * Function: remove_card
 * ---------------------
 *  Remove a card from the player's hand. 
 *
 *  target: the target player
 *  old_card: pointer to the old card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card){

    struct hand* iterator = target->card_list;
    struct hand* previous = NULL;
    if (iterator == NULL){
        return 0;
    }
    while (iterator != NULL){
        if (iterator->top.rank[0] == old_card->rank[0] && iterator->top.suit == old_card->suit){
            if (previous == NULL){
                target->card_list = iterator->next;
            }
            else{
                previous->next = iterator->next;
            }
            free(iterator);
            target->hand_size--;
            return 1;
        }
        previous = iterator;
        iterator = iterator->next;
    }
    return 0;

}

/*
 * Function: countOccurences
 * ----------------------
 *  Count the number of occurences of a card in a player's hand.
 *
 *  target: the target player
 *  rank: the rank of the card to count
 *
 *  returns: the number of occurences of the card in the player's hand
 */
int count_rank_occurance(struct player* target, char rank){
    int count = 0;
    struct hand* iterator = target->card_list;
    while(iterator != NULL){
        if(iterator->top.rank[0] == rank){
            count++;
        }
        iterator = iterator->next;
    }
    return count;
}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 *  target: pointer to the player to check
 *  
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 */

char check_add_book(struct player* target){

    struct hand* temp;
    struct hand* temp1;
    temp = target->card_list;
    temp1 = target->card_list;
    char rank;

    //check if there are 4 cards of the same rank
    while(temp != NULL){
        rank = temp->top.rank[0]; // get the rank of the card
        if(count_rank_occurance(target, rank) == 4){
            while(temp1 != NULL){
                if(temp1->top.rank[0] == rank){
                    remove_card(target, &temp1->top);
                }
                temp1 = temp1->next;
            }
            int i = 0;
            while(target->book[i] != 0){
                i++;
            }
            target->book[i] = rank;

            if (target == &user){
                printf("\t- Player 1 books %c", rank);
                printf("\n");
            }
            else{
                printf("\t- Player 2 books %c", rank);
                printf("\n");
            }
            return rank;
        }
        temp = temp->next;
    }
    return 1;

}

/*
 * Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *  
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 */
int search(struct player* target, char rank){
    struct hand* iterator = target->card_list;
    while(iterator != NULL){
        if(iterator->top.rank[0] == rank){
            return 1;
        }
        iterator = iterator->next;
    }
    return 0;
}

/*
 * Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's 
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *   
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise 
 *   return value indicates number of cards transferred
 */   
int transfer_cards(struct player* src, struct player* dest, char rank){
    struct hand* iterator = src->card_list;
    int count = 0;
    while(iterator != NULL){
        if(iterator->top.rank[0] == rank){
            add_card(dest, &iterator->top);
            remove_card(src, &iterator->top);
            count++;
        }
        iterator = iterator->next;
    }
    return count;
}

/*
 * Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *   
 *   Return: 1 if game is over, 0 if game is not over
 */
int game_over(struct player* target){
    int i = 0;
    while(target->book[i] != 0){
        i++;
    }
    if(i == 7){
        return 1;
    }
    return 0;
}

/* 
 * Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 * 
 *   target: player to reset
 * 
 *   Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target){
    struct hand* iterator = target->card_list;
    while(iterator != NULL){
        struct hand* temp = iterator;
        iterator = iterator->next;
        free(temp);
    }
    target->card_list = NULL;
    target->hand_size = 0;
    int i = 0;

    while(target->book[i] != 0){
        target->book[i] = 0;
        i++;
    }
    return 0;
}

/* 
 * Function: computer_play
 * -----------------------
 *
 *   Select a rank randomly to play this turn. The player must have at least
 *   one card of the selected rank in their hand.
 *
 *   target: the player's hand to select from
 *
 *   Rank: return a valid selected rank
 */
char computer_play(struct player* target){
    char choice[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
    int i = rand() % 13;
    while(search(target, choice[i]) == 0){
        i = rand() % 13;
    }
    return choice[i];
    
}

/* 
 * Function: user_play
 * -------------------
 *
 *   Read standard input to get rank user wishes to play.  Must perform error
 *   checking to make sure at least one card in the player's hand is of the 
 *   requested rank.  If not, print out "Error - must have at least one card from rank to play"
 *   and then re-prompt the user.
 *
 *   target: the player's hand to check
 * 
 *   returns: return a valid selected rank
 */
char user_play(struct player* target){
    char rank;
    printf("Enter a rank: ");
    scanf(" %c", &rank);
    while(search(target, rank) == 0){
        printf("Error - must have at least one card from rank to play\n\n");
        printf("Enter a rank: ");
        scanf(" %c", &rank);
    }
    return rank;
}


