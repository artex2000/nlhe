#include <stdio.h>
#include <string.h>

#include "nlhe.h"

// reserve 0, since ranks start with 1, ie. deuce - 1, three - 2, etc.
static const char print_rank[] = {
    0, '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'
};

static const char *single_rank_name[] = {
    "none", "deuce", "three", "four", "five", "six", "seven",
    "eight", "nine", "ten", "jack", "queen", "king", "ace"
};

static const char *plural_rank_name[] = {
    "none", "deuces", "threes", "fours", "fives", "sixes", "sevens",
    "eights", "nines", "tens", "jacks", "queens", "kings", "aces"
};

static const u16 card_bit[] = {
    0x0000,
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000
};

static const u16 straight_template[] = {
    /*AKQJT98765432*/
    0b0000000011111,    //straight 2 to 6
    0b0000000111110,
    0b0000001111100,
    0b0000011111000,
    0b0000111110000,
    0b0001111100000,
    0b0011111000000,
    0b0111110000000,
    0b1111100000000,
    0b1000000001111     //straight A to 5
};

static const hand_rank_t zero_hand_rank = {
    0,0,0,0,0,0,0,0
};

static u8 char_to_rank (char c)
{
    if (c >= '2' && c <= '9')
        return c - '2';

    switch (c) {
        case 't':
        case 'T':
            return 8;
        case 'j':
        case 'J':
            return 9;
        case 'q':
        case 'Q':
            return 10;
        case 'k':
        case 'K':
            return 11;
        case 'a':
        case 'A':
            return 12;
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
            return 0;
        case 'h':
        case 'H':
            return 1;
        case 'c':
        case 'C':
            return 2;
        case 'd':
        case 'D':
            return 3;
        default:
            break;
    }
    return 0xff;
}

static void print_card (card_t card)
{
    switch (card.suit) {
        case 0:
            printf ("%c\u2660 ", print_rank[card.rank]);
            break;
        case 1:
            printf ("%c\u2665 ", print_rank[card.rank]);
            break;
        case 2:
            printf ("%c\u2663 ", print_rank[card.rank]);
            break;
        case 3:
            printf ("%c\u2666 ", print_rank[card.rank]);
            break;
        default:
            break;
    }
}

static void print_hand (hand_t *hand)
{
    print_card (hand->card[0]);
    print_card (hand->card[1]);
    printf ("\n");
}

static void print_made_hand (made_hand_t *hand)
{
    print_card (hand->card[0]);
    print_card (hand->card[1]);
    print_card (hand->card[2]);
    print_card (hand->card[3]);
    print_card (hand->card[4]);
    printf ("\n");
}

static void get_hand (hand_t *hand)
{
    char a[3];
    char b[3];
    scanf ("%s %s", a, b);
    hand->card[0].rank = char_to_rank (a[0]);
    hand->card[0].suit = char_to_suit (a[1]);
    hand->card[0].bit = card_bit[hand->card[0].rank];
    hand->card[1].rank = char_to_rank (b[0]);
    hand->card[1].suit = char_to_suit (b[1]);
    hand->card[1].bit = card_bit[hand->card[1].rank];
}

static void get_made_hand (made_hand_t *hand)
{
    char a[3];
    int i;
    for (i = 0; i < 5; i++) {
        scanf ("%s", a);
        hand->card[i].rank = char_to_rank (a[0]);
        hand->card[i].suit = char_to_suit (a[1]);
        hand->card[i].bit = card_bit[hand->card[i].rank];
    }
}

static void hand_to_value (made_hand_t *hand, hand_value_t *value)
{
    int i;
    memset (value, 0, sizeof (*value));
    for (i = 0; i < 5; i++) {
        value->straight |= hand->card[i].bit;
        value->flush[hand->card[i].suit]++;
        value->ranks[hand->card[i].rank]++;
    }
}

static u8 is_flush (hand_value_t *value)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (value->flush[i] == 5)
            return 1;
    }
    return 0;
}

static u8 is_straight (hand_value_t *value)
{
    int i;
    for (i = 0; i < 9; i++) {
        if (value->straight == straight_template[i])
            return 1;
    }
    return (value->straight == straight_template[9]) ? 2 : 0;
}

static void comprehend_rank (hand_rank_t *rank)
{
    if (rank->straight && rank->flush) {
        if (rank->straight == 1 && rank->high_card == 13)
            rank->rank = ROYAL_FLUSH;
        else
            rank->rank = STRAIGHT_FLUSH;
    } else if (rank->flush) {
        rank->rank = FLUSH;
    } else if (rank->straight) {
        rank->rank = STRAIGHT;
    } else if (rank->four_kind) {
        rank->rank = FOUR_OF_KIND;
    } else if (rank->three_kind && rank->high_pair) {
        rank->rank = FULL_HOUSE;
    } else if (rank->three_kind) {
        rank->rank = THREE_OF_KIND;
    } else if (rank->high_pair && rank->low_pair) {
        rank->rank = TWO_PAIRS;
    } else if (rank->high_pair) {
        rank->rank = PAIR;
    } else {
        rank->rank = HIGH_CARD;
    }
}

static hand_rank_t value_to_rank (hand_value_t *value)
{
    int i, p = 0;
    hand_rank_t rank = zero_hand_rank;
    rank.flush = is_flush (value);
    rank.straight = is_straight (value);
    for (i = 13; i > 0; i--) {
        switch (value->ranks[i]) {
            case 4:
                rank.four_kind = i;
                break;
            case 3:
                rank.three_kind = i;
                break;
            case 2:
                if (p == 0) {
                    rank.high_pair = i;
                    p++;
                } else {
                    rank.low_pair = i;
                }
                break;
            case 1:
                if (rank.high_card == 0)
                    rank.high_card = i;
                break;
            default:
                break;
        }
    }
    comprehend_rank (&rank);
    return rank;
}

int main (void)
{
    made_hand_t hand;
    printf ("enter made hand: ");
    get_made_hand (&hand);
    print_made_hand (&hand);
    return 0;
}
