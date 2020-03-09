// Assignment 1 19T1 COMP1511 Coco
//
// This program was written by Nguyen Hoang (z5257125)
// on 3rd April 2019
//

#include <stdio.h>
#include <assert.h>

#define ACTION_PLAYER_NAME    0
#define ACTION_DISCARD        1
#define ACTION_PLAY_CARD      2

#define N_CARDS              40
#define N_CARDS_INITIAL_HAND 10
#define N_PLAYERS             4
#define N_CARDS_DISCARDED     3

#define CARD_MIN             10
#define CARD_MAX             49

// IF YOU NEED MORE #defines ADD THEM HERE
#define N_INITIAL_TRICK_SIZE 3
#define N_INFO_CARDS         3
#define DOUGLAS              42

void print_player_name(void);
void choose_discards(void);
void choose_card_to_play(void);


// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
int prime_checker(int i, int cards[]);
int douglas_check(int total_played, int card_history[]);
int cocomposite(int position, int hand[], int trick[]);
int douglas_check_trick(int trick_size, int trick[]);
void scan(int card, int array[]);
void douglas(int hand_size, int hand[], int trick_size, int trick[]);
void coco_play(int hand_size, int hand[], int trick_size, int trick[]);
void prime(int hand_size, int hand[], int trick_size, int trick[]);
void first_round(int hand_size, int hand[], int trick_size, int trick[]);
void playing_first(int hand_size, int hand[]);
// You should not need to change this main function

int main(void) {

    int which_action = 0;
    scanf("%d", &which_action);
    if (which_action == ACTION_PLAYER_NAME) {
        print_player_name();
    } else if (which_action == ACTION_DISCARD) {
        choose_discards();
    } else if (which_action == ACTION_PLAY_CARD) {
        choose_card_to_play();
    }

    return 0;
}

void print_player_name(void) {

    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME

    printf("NAHGOOYEN");

}

void choose_discards() {
    int i = 0;
    int cards[N_CARDS_INITIAL_HAND] = {0};
    int card1 = 0;
    int card2 = 0;
    int card3 = 0;
//scan cards in hand
    while (i < N_CARDS_INITIAL_HAND) {
        scanf("%d", &cards[i]);
        i++;
    }
    i = 0;
//test for largest prime number
    while (i < N_CARDS_INITIAL_HAND) {
        if (prime_checker(i, cards)) {
            card1 = cards[i];
        }
        i++;
    }
    i = 0;
//test for second largest prime number
    while (i < N_CARDS_INITIAL_HAND) {
        if (prime_checker(i, cards) && cards[i] != card1) {
            card2 = cards[i];
        }
        i++;
    }
    i = 0;
//if card1 and card2 less than 30 test for larger non prime cards
    if (card1 < 30) {
        while (i < N_CARDS_INITIAL_HAND) {
            if (cards[i] > card1 && cards[i] != card2) {
                card1 = cards[i];
            }
            i++;
        }
    }
    i = 0;
    if (card2 < 30) {
        while (i < N_CARDS_INITIAL_HAND) {
            if (cards[i] > card2 && cards[i] != card1) {
                card2 = cards[i];
            }
            i++;
        }
    }
    i = 0;
//test for largest non prime number that is not card1 or card2
    while (i < N_CARDS_INITIAL_HAND) {
        if (cards[i] > card3 && cards[i] != card1 && cards[i] != card2) {
            card3 = cards[i];
        }
        i++;
    }
    i = 0;
    while (i < N_CARDS_INITIAL_HAND) { //test for douglas and discard
        if (cards[i] != card1 && cards[i] != card2 && cards[i] == 42) {
            card3 = cards[i];
        }
        i++;
    }

    printf("%d %d %d\n", card3, card2, card1);
}


void choose_card_to_play(void) {
//Initialise variables
    int i = 0;
    int position;
    int hand_size;
    int trick_size; // trick_size is the number of cards played this round
    int total_played;
    int info_cards[N_INFO_CARDS] = {0};
    int hand[N_CARDS_INITIAL_HAND] = {0};
    int trick[N_INITIAL_TRICK_SIZE] = {0};
    int card_history[CARD_MAX] = {0};
    int discarded[N_CARDS_DISCARDED] = {0};
    int recieved[N_CARDS_DISCARDED] = {0};
//scan first three cards for hand size, how many cards in trick and position
    scan(N_INFO_CARDS, info_cards);
//reinitialise variables
    hand_size = info_cards[0];
    trick_size = info_cards[1];
    position = info_cards[2];
    total_played = ((N_CARDS_INITIAL_HAND - hand_size) * N_PLAYERS);
//scan for cards in hand, cards in trick, cards played,
    scan(hand_size, hand);
    scan(trick_size, trick);
    scan(total_played, card_history);
    scan(N_INFO_CARDS, discarded);
    scan(N_INFO_CARDS, recieved);

//if this is the first round
    if (hand_size == 10) {
        first_round(hand_size, hand, trick_size, trick);
    } else if (trick_size == 0) { //if you are going first in the round
        if (douglas_check(total_played, card_history)) {
            playing_first(hand_size, hand);
        } else {//Otherwise use normal strategy
            coco_play(hand_size, hand, trick_size, trick);
        }
    }
//If the first card played was a prime number
    else if (prime_checker(i, trick)) {
        prime(hand_size, hand, trick_size, trick);
    }
//If this round has a 42 played use this strategy
    else if (douglas_check_trick(trick_size, trick)) {
        douglas(hand_size, hand, trick_size, trick);
    }
    else { //Otherwise use this strategy
        coco_play(hand_size, hand, trick_size, trick);
    }
}

// ADD YOUR FUNCTIONS HERE
int prime_checker(int i, int cards[]) {
    int counter1 = 2;
    int x = 0;
    while (counter1 <= cards[i]) { //loop through numbers and check if card is prime
        if (cards[i] % counter1 == 0) {
            x++;
        }
        counter1++;
    }
    if (x == 1) {
        return 1;
    }
    return 0;
}

void scan(int card, int array[]) {
    int counter = 0;
    while (counter < card) { //general scanning loop to place numbers into an array
        scanf("%d", &array[counter]);
        counter++;
    }
}

int douglas_check_trick(int trick_size, int trick[]) {
    int i = 0;
    //loop through cards played this round and return 1 if there is douglas
    while (i < trick_size) {
        if (trick[i] == DOUGLAS) {
            return 1;
        }
        i++;
    }
    return 0;
}

int douglas_check(int total_played, int card_history[]) {
    int i = 0;
    //loop through cards played previously and return 0 if there is douglas
    while (i < total_played) {
        if (card_history[i] == DOUGLAS) {
            return 0;
        }
        i++;
    }
    return 1;
}

int cocomposite(int position, int hand[], int trick[]) {
    int z = 2;
    while (z <= 49) { //Loop through cards and return 1 if the card is cocomposite
        if (hand[position] % z == 0 && trick[0] % z == 0
        && hand[position] != z && trick[0] != z) {
            return 1;
        }
        z++;
    }
    return 0;
}

 int prime_trick_check(int trick_size, int trick[]) {
    int i = 0;
    while (i < trick_size) {
        if (prime_checker(i, trick)) {
            return 1;
        }
        i++;
    }
    return 0;
}

void coco_play(int hand_size, int hand[], int trick_size, int trick[]) {
    int i = 0;
    int card = 0;
    int y = 0;
    if (trick_size > 0) { //find the largest number in the trick that is cocomposite
        while (i < trick_size) {
            if (y < trick[i] && cocomposite(i, trick, trick)) {
                y = trick[i];
            }
            i++;
        }
        i = 0;
    }
    if (prime_trick_check(trick_size, trick) == 0 && trick_size > 1) {
        while (i < hand_size) {
            if (cocomposite(i, hand, trick) == 1 && hand[i] > card) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//find highest possible coco card without winning for rounds excluding 1 and 2
    if ((hand_size != 9 || hand_size != 8) && card == 0) {
        while (i < hand_size) {
            if (cocomposite(i, hand, trick) == 1 && hand[i] < y && hand[i] > card) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//find highest coco card
    if (card == 0) {
        while (i < hand_size) {
            if (cocomposite(i, hand, trick) == 1 && hand[i] > card) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//find largest prime number
    if (card == 0) {
        while (i < hand_size) {
            if (prime_checker(i, hand)) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//find largest card
    if (card == 0) {
        while (i < hand_size) {
            if (hand[i] > card) {
                card = hand[i];
            }
            i++;
        }
    }
    printf("%d\n", card);
}

void douglas(int hand_size, int hand[], int trick_size,
            int trick[]) {
    int i = 0;
    int card = 0;

//find card that is cocomposite with leader and the smallest
    while (i < hand_size) {
        if (cocomposite(i, hand, trick) && hand[i] < DOUGLAS && hand[i] > card) {
            card = hand[i];
        }
        i++;
    }
    i = 0;
//Otherwise find card that is cocomposite with leader
    if (card == 0) {
        while (i < hand_size) {
            if (cocomposite(i, hand, trick) && hand[i] > card) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//play largest prime number
    if (card == 0) {
        while (i < hand_size) {
            if (prime_checker(i, hand)) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//If prime, find smallest prime possible or play largest card less than 42
    if (card == 0 && prime_checker(i, trick) == 1) {
        while (i < hand_size) {
            if (prime_checker(i, hand)) {
                card = hand[i];
                printf("%d\n", card);
                return;
            }
            i++;
        }
        i = 0;
    } else if (card == 0 && prime_checker(i, trick) == 1) {
        while (i < hand_size) {
            if (hand[i] < DOUGLAS) {
                card = hand[i];
            }
            i++;
        }
    } if (card == 0) { // play smallest possible card
        card = hand[0];
    }
    printf("%d\n", card);
}

void prime(int hand_size, int hand[], int trick_size, int trick[]) {
    int i = 0;
    int card = 0;
    int x = 0;
//find largest number in trick so far
    while (i < trick_size) {
        if (x < trick[i] && prime_checker(i, trick) == 1) {
            x = trick[i];
        }
        i++;
    }
    i = 0;
//play lowest prime number
    while (i < hand_size) {
        if (prime_checker(i, hand) == 1 && hand[i] < x) {
            card = hand[i];
        }
        i++;
    }
    i = 0;
//play any prime number
    if (card == 0) {
        while (i < hand_size) {
            if (prime_checker(i, hand)) {
                card = hand[i];
                printf("%d\n", card);
                return;
            }
            i++;
        }
        i = 0;
    }
//play number smaller than largest in trick
    if (card == 0) {
        while (i < hand_size) {
            if (hand[i] < x) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//play lowest number
    if (card == 0) {
        card = hand[0];
    }
    printf("%d\n", card);
}

void first_round(int hand_size, int hand[], int trick_size, int trick[]) {
    int i = 0;
    int card = 0;
//if going first on first round
    if (trick_size == 0) {
        while (i < hand_size) {
            if (hand[i] < DOUGLAS && prime_checker(i, hand) == 0) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
//if going second third
    if (trick_size > 0 && trick_size < 3) {
        while (i < hand_size) {
            if (cocomposite(i, hand, trick)) {
                if (douglas_check_trick(trick_size, trick) == 1 && hand[i] < DOUGLAS) {
                    card = hand[i];
                } else if (card == 0) {
                    card = hand[i];
                }
            }
            i++;
        }
        i = 0;
    }
//if going last
    if (trick_size == 3) {
        while (i < hand_size) {
            if (douglas_check_trick(trick_size, trick) == 1) {
                if (cocomposite(i, hand, trick)) {
                    if (hand[i] < DOUGLAS) {
                        card = hand[i];
                    }
                }
            } else {
                if (cocomposite(i, hand, trick)) {
                    card = hand[i];
                }
            }
            i++;
        }
        i = 0;
    }
    if (card == 0) {
        while (i < hand_size) {
            if (prime_checker(i, hand)) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    }
    if (card == 0) {
        while (i < hand_size) {
            if (hand[i] > card) {
                card = hand[i];
            }
            i++;
        }
    }
    printf("%d\n", card);
}

void playing_first(int hand_size, int hand[]) {
    int i = 0;
    int card = 0;
//Find the largest card less than 42 and is not prime
    while (i < hand_size) {
        if (prime_checker(i, hand) == 0 && hand[i] < DOUGLAS) {
            card = hand[i];
        }
        i++;
    }
    i = 0;
    if (card == 0) { //Find largest non prime card
        while (i < hand_size) {
            if (prime_checker(i, hand) == 0) {
                card = hand[i];
            }
            i++;
        }
        i = 0;
    } if (card == 0) {
        while (i < hand_size) { //Find largest card
            card = hand[i];
            i++;
        }
    }
    printf("%d\n", card);
}


