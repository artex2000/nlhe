#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "nlhe.h"

void rand_init (void)
{
    srand (time (NULL));
}

card_t random_card (deck_t *deck)
{
    card_t r;

    do {
        r.suit = rand() % 4;
        do {
            r.rank = rand () % 14;
        } while (r.rank == 0);
    } while (!is_in_deck (deck, r));
    r.bit = card_bit[r.rank];
    return r;
}

void random_hand (deck_t *deck, hand_t *hand)
{
    hand->card[0] = random_card (deck);
    hand->card[1] = random_card (deck);
}

void random_board (deck_t *deck, board_t *board)
{
    int i;
    for (i = 0; i < 5; i++)
        board->card[i] = random_card (deck);
}

    
