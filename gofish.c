#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deck.h"
#include "player.h"
#include "card.h"
#include "gofish.h"

/*
 * Function: print_book
 * ----------------
 *  Print the book of the target player.
 *  
 *  target: the player (and their book) to print
 */
void print_book(struct player* target){ 

	for(int i = 0; i < 7; i++){
		printf(" %c ",target->book[i]); //Print each target player book
	}
  printf("\n");
}
/*
*  Function: print_hand
*  ----------------
*  Print all cards in the hand of the target player.
*  
*  target: the player (and their cards) to print
*/
void print_hand(struct player* target){
    struct hand* iterator = target->card_list;
    while (iterator != NULL){
        printf("%c%c ", iterator->top.rank[0], iterator->top.suit);
        iterator = iterator->next;
    }
    printf("\n");
}

/*
 * Function: print_cards_in_hand
 * ----------------
 *  Print the cards of the rank requested in the hand of the target player.
 * 
 *  target: the player (and their cards) to print
 *  rank: the rank of the cards to print
 */
void print_cards_in_hand(struct player* target, char rank){ 
  struct hand* iterator = target->card_list;
  while (iterator != NULL){
    if (iterator->top.rank[0] == rank){
      printf("%c%c ", iterator->top.rank[0], iterator->top.suit);
    }
    iterator = iterator->next;
  }
  printf("\n");
}

/*
* Function: count_books
* ----------------
*  count the number of books in the target player.
* 
*  target: the player (and their books) to count
*
*  Return: the number of books in the target player
*/
int count_books(struct player* target){
  int count = 0;
  for (int i = 0; i < 7; i++){
    if (target->book[i] != 0){
      count++;
    }
  }
  return count;
}


int main(int args, char* argv[]){
  int flag_count_book_user = 1;
  int flag_count_book_computer = 1;

  do{ // game loop

    printf("Shuffling deck...\n\n");
    shuffle(); 


    deal_player_cards(&user);
    deal_player_cards(&computer);
    int flag_user = 0; //Check is user has a card in hand
    int flag_computer = 0; //Check is computer has a card in hand
    do{

      while (1){ // user turn

        printf("Player 1's Hand - ");
        print_hand(&user);
        printf("Player 1's Books - ");
        print_book(&user);
        printf("Player 2's Books - ");
        print_book(&computer);

        printf("Player 1's turn - ");
        char rank = user_play(&user);

        if(search(&computer, rank)){ // if computer has the rank

          printf("\t- Player 2 has ");
          print_cards_in_hand(&computer, rank);

          printf("\t- Player 1 has ");
          print_cards_in_hand(&user, rank);

          transfer_cards(&computer, &user, rank);
          check_add_book(&user);
          if (count_books(&user) == flag_count_book_user){
            flag_count_book_user++;
          }

          if(game_over(&user)){ // checking if the game is over
            break;
          }
          printf("\t- Player 1 gets another turn");

        }

        else{ // if computer does not have the rank, user goes fish 
          printf("\t- Player 2 has no %c's", rank);
          printf("\n\t- Go Fish!");
          struct card* new_card = next_card(); // get the next card from the deck

          printf(" Player 1 draws %c%c\n", new_card->rank[0], new_card->suit); // print the card that was drawn
          add_card(&user, new_card);
          check_add_book(&user);
          if(game_over(&user)){
            break;
          }
          // check if new card is same as rank asked for
          if (new_card->rank[0] == rank){
            printf("\t- Player 1 gets another turn");
          }
          else if (count_books(&user) == flag_count_book_user){
            printf("\t- Player 1 gets another turn");
            flag_count_book_user++;
          }
          else{
            printf("\t- Player 2's turn\n");
            printf("\n\n");
            break;
          }
        }
        
        //check if cards in hand is 0
        if (user.card_list == NULL){
          flag_user = 1;
          break;
        }
        printf("\n\n");
      }

      if (game_over(&user) || game_over(&computer)){
        break;
      }
      
      // computer's turn. Computer is always player 2
      while(1){
        printf("Player 1's Hand - ");
        print_hand(&user);
        printf("Player 1's Books - ");
        print_book(&user);
        printf("Player 2's Books - ");
        print_book(&computer);

        char rank = computer_play(&computer);
        printf("Player 2's turn, enter a Rank: %c\n", rank);

        if(search(&user, rank)){
          printf("\t- Player 1 has ");
          print_cards_in_hand(&user, rank);

          printf("\t- Player 2 has ");
          print_cards_in_hand(&computer, rank);

          transfer_cards(&user, &computer, rank);
          check_add_book(&computer);
          if (count_books(&computer) == flag_count_book_computer){
            flag_count_book_computer++;
          }
          if (game_over(&computer)){
            break;
          }
          printf("\t- Player 2 gets another turn");
        }

        else{
          printf("\t- Player 1 has no %c's", rank);
          printf("\n\t- Go Fish!");
          struct card* new_card = next_card();
          printf(" Player 2 draws a card\n");
          add_card(&computer, new_card);
          check_add_book(&computer);

          if (game_over(&computer)){ // checking if the game is over
            break;
          }

          if (new_card->rank[0] == rank){
            printf("\t- Player 2 gets another turn");
          }
          // check if a book was added and if so, give computer another turn
          else if (count_books(&computer) == flag_count_book_computer){
            printf("\t- Player 2 gets another turn");
            flag_count_book_computer++;
          }
          else{
            printf("\t- Player 1's turn\n");
            printf("\n\n");
            break;
          }

        }

        printf("\n\n");
        if (computer.card_list == NULL){ // check if computer has cards in hand
          flag_computer = 1;
          break;
        }
      }

      if (game_over(&user) || game_over(&computer)){
        break;
      }

    }while(1);

    // check if game is over
    if(game_over(&user)){
      printf("\n\n");

      printf("Player 1 wins! %d - %d \n", count_books(&user), count_books(&computer));
      printf("Player 1's Books - ");
      print_book(&user);
      printf("Player 2's Books - ");
      print_book(&computer);
    }
    else if(game_over(&computer)){
      printf("Player 2 wins! %d - %d \n", count_books(&computer), count_books(&user));
      printf("Player 1's Books - ");
      print_book(&user);
      printf("Player 2's Books - ");
      print_book(&computer);
    }
    else if (flag_user == 1){
      printf("Player 1 has no cards left. Player 2 wins! \n");
    }
    else if (flag_computer == 1){
      printf("Player 2 has no cards left. Player 1 wins! \n");
    }

    // check if user wants to play again
    char input;
    printf("Do you want to play again [Y/N]: ");
    scanf(" %c", &input);

    if (input == 'Y' || input == 'y'){
      reset_player(&user);
      reset_player(&computer);
      continue;

    }
    else if (input == 'N' || input == 'n'){
      printf("Thanks for playing!\n");
      break;
    }
  }while(1);

  return 0;
}
