#include <stdio.h>

#include "nlhe.h"

const u16 card_bit[] = {
    0x0000,
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000
};

static u8 char_to_rank (char c)
{
    if (c >= '2' && c <= '9')
        return c - '1';

    switch (c) {
        case 't':
        case 'T':
            return TEN;
        case 'j':
        case 'J':
            return JACK;
        case 'q':
        case 'Q':
            return QUEEN;
        case 'k':
        case 'K':
            return KING;
        case 'a':
        case 'A':
            return ACE;
        default:
            break;
    }
    return 0xff;
}

static u8 char_to_suit (char c)
{
    switch (c) {
        case 's':
        case 'S':
            return SPADES;
        case 'h':
        case 'H':
            return HEARTS;
        case 'c':
        case 'C':
            return CLUBS;
        case 'd':
        case 'D':
            return DIAMONDS;
        default:
            break;
    }
    return 0xff;
}

void get_hand (hand_t *hand)
{
    char a[3];
    char b[3];
    u8 ar, br, as, bs;

    printf ("Enter hand: ");
    scanf ("%s %s", a, b);
    ar = char_to_rank (a[0]);
    as = char_to_suit (a[1]);
    br = char_to_rank (b[0]);
    bs = char_to_suit (b[1]);
    if (ar >= br) {
        hand->card[0].rank = ar;
        hand->card[0].suit = as;
        hand->card[0].bit = card_bit[ar];
        hand->card[1].rank = br;
        hand->card[1].suit = bs;
        hand->card[1].bit = card_bit[br];
    } else {
        hand->card[0].rank = br;
        hand->card[0].suit = bs;
        hand->card[0].bit = card_bit[br];
        hand->card[1].rank = ar;
        hand->card[1].suit = as;
        hand->card[1].bit = card_bit[ar];
    }
}

void get_board (board_t *board)
{
    char a[3];
    int i;
    printf ("Enter board: ");
    for (i = 0; i < 5; i++) {
        scanf ("%s", a);
        board->card[i].rank = char_to_rank (a[0]);
        board->card[i].suit = char_to_suit (a[1]);
        board->card[i].bit = card_bit[board->card[i].rank];
    }
}


