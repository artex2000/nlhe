#include <stdio.h>

#include "nlhe.h"

// reserve 0, since ranks start with 1, ie. deuce - 1, three - 2, etc.
static const char print_card_rank[] = {
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

//static void print_card (card_t card)
void print_card (card_t card)
{
    switch (card.suit) {
        case SPADES:
            printf ("%c\u2660 ", print_card_rank[card.rank]);
            break;
        case HEARTS:
            printf ("%c\u2665 ", print_card_rank[card.rank]);
            break;
        case CLUBS:
            printf ("%c\u2663 ", print_card_rank[card.rank]);
            break;
        case DIAMONDS:
            printf ("%c\u2666 ", print_card_rank[card.rank]);
            break;
        default:
            break;
    }
}

void print_rank (hand_rank_t *rank)
{
    switch (rank->rank) {
        case ROYAL_FLUSH:
            printf ("royal flush\n");
            break;
        case STRAIGHT_FLUSH:
            printf ("straight flush %s high\n",
                    single_rank_name[rank->straight]);
            break;
        case FOUR_OF_KIND:
            printf ("four %s with %s kicker\n",
                    plural_rank_name[rank->four_kind],
                    single_rank_name[rank->high_card[0]]);
            break;
        case FULL_HOUSE:
            printf ("full house %s full of %s\n",
                    plural_rank_name[rank->three_kind],
                    plural_rank_name[rank->high_pair]);
            break;
        case FLUSH:
            printf ("flush %s high\n",
                    single_rank_name[rank->high_card[0]]);
            break;
        case STRAIGHT:
            printf ("straight %s high\n",
                    single_rank_name[rank->straight]);
            break;
        case THREE_OF_KIND:
            printf ("three %s with %s kicker\n",
                    plural_rank_name[rank->three_kind],
                    single_rank_name[rank->high_card[0]]);
            break;
        case TWO_PAIRS:
            printf ("two pairs %s and %s with %s kicker\n",
                    plural_rank_name[rank->high_pair],
                    plural_rank_name[rank->low_pair],
                    single_rank_name[rank->high_card[0]]);
            break;
        case PAIR:
            printf ("pair of %s with %s kicker\n",
                    plural_rank_name[rank->high_pair],
                    single_rank_name[rank->high_card[0]]);
            break;
        case HIGH_CARD:
            printf ("high card %s\n",
                    single_rank_name[rank->high_card[0]]);
            break;
        default:
            break;
    }
}

void print_hand (hand_t *hand)
{
    print_card (hand->card[0]);
    print_card (hand->card[1]);
    printf ("\n");
}

void print_board (board_t *board)
{
    print_card (board->card[0]);
    print_card (board->card[1]);
    print_card (board->card[2]);
    print_card (board->card[3]);
    print_card (board->card[4]);
    printf ("\n");
}

void print_meta (meta_hand_t m)
{
    char s[4];
    s[0] = print_card_rank[m.rank1];
    s[1] = print_card_rank[m.rank2];
    if (m.rank1 == m.rank2) {
        s[2] = 0;
    } else {
        s[2] = (m.suited) ? 's' : 'o';
        s[3] = 0;
    }
    printf ("%s\n", s);
}

void print_hand_as_meta (hand_t *hand)
{
    meta_hand_t m;
    m.rank1 = hand->card[0].rank;
    m.rank2 = hand->card[1].rank;
    m.suited = (hand->card[0].suit == hand->card[1].suit) ? 1 : 0;
    m.reserved = 0;
    print_meta (m);
}
