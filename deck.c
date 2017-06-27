#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlhe.h"

void init_deck (deck_t *deck)
{
    deck->set = malloc (56);    //4 * 14 as ranks go for 1 - 13
    memset (deck->set, 0, 56);
    deck->size = 52;
}

void remove_card (deck_t *deck, card_t card)
{
    int idx;
    idx = card.suit * 14 + card.rank;
    deck->set[idx] = 1;
    deck->size--;
}

void remove_hand (deck_t *deck, hand_t *hand)
{
    remove_card (deck, hand->card[0]);
    remove_card (deck, hand->card[1]);
}

void remove_flop (deck_t *deck, flop_t *hand)
{
    remove_card (deck, hand->card[0]);
    remove_card (deck, hand->card[1]);
    remove_card (deck, hand->card[2]);
}

void remove_board (deck_t *deck, board_t *hand)
{
    remove_card (deck, hand->card[0]);
    remove_card (deck, hand->card[1]);
    remove_card (deck, hand->card[2]);
    remove_card (deck, hand->card[3]);
    remove_card (deck, hand->card[4]);
}

int deck_to_cards (deck_t *deck, card_t **cards)
{
    card_t cr, *tmp;
    int i, j, c = 0;

    tmp = malloc (sizeof (card_t) * deck->size);
    for (i = 0; i < 4; i++) {
        for (j = 1; j < 14; j++) {
            if (deck->set[i * 14 + j] == 0) {
                cr.rank = j;
                cr.suit = i;
                cr.bit = card_bit[j];
                tmp[c++] = cr;
            }
        }
    }
    *cards = tmp;
    return c;
}

int is_in_deck (deck_t *deck, card_t card)
{
    int idx;
    idx = card.suit * 14 + card.rank;
    return (deck->set[idx] == 0) ? 1 : 0;
}

