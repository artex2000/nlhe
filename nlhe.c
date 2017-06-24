#include <stdio.h>
#include <string.h>

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

static const u16 card_bit[] = {
    0x0000,
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000
};

static const u16 straight_template[] = {
    /*AKQJT98765432*/
    0b1111100000000,
    0b0111110000000,
    0b0011111000000,
    0b0001111100000,
    0b0000111110000,
    0b0000011111000,
    0b0000001111100,
    0b0000000111110,
    0b0000000011111,    //straight 2 to 6
    0b1000000001111     //straight A to 5
};

static const hand_rank_t zero_hand_rank = {
    0,0,0,0,0,0,0,0
};

static u8 char_to_rank (char c)
{
    if (c >= '2' && c <= '9')
        return c - '1';

    switch (c) {
        case 't':
        case 'T':
            return 9;
        case 'j':
        case 'J':
            return 10;
        case 'q':
        case 'Q':
            return 11;
        case 'k':
        case 'K':
            return 12;
        case 'a':
        case 'A':
            return 13;
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
            printf ("%c\u2660 ", print_card_rank[card.rank]);
            break;
        case 1:
            printf ("%c\u2665 ", print_card_rank[card.rank]);
            break;
        case 2:
            printf ("%c\u2663 ", print_card_rank[card.rank]);
            break;
        case 3:
            printf ("%c\u2666 ", print_card_rank[card.rank]);
            break;
        default:
            break;
    }
}

static void print_rank (hand_rank_t *rank)
{
    switch (rank->rank) {
        case ROYAL_FLUSH:
            printf ("You have royal flush\n");
            break;
        case STRAIGHT_FLUSH:
            printf ("You have straight flush %s high\n",
                    single_rank_name[rank->high_card[0]]);
            break;
        case FOUR_OF_KIND:
            printf ("You have four %s with %s kicker\n",
                    plural_rank_name[rank->four_kind],
                    single_rank_name[rank->high_card[0]]);
            break;
        case FULL_HOUSE:
            printf ("You have full house %s full of %s\n",
                    plural_rank_name[rank->three_kind],
                    plural_rank_name[rank->high_pair]);
            break;
        case FLUSH:
            printf ("You have flush %s high\n",
                    single_rank_name[rank->high_card[0]]);
            break;
        case STRAIGHT:
            printf ("You have straight %s high\n",
                    (rank->straight == 1) ? //regular straight?
                        single_rank_name[rank->high_card[0]] :
                        single_rank_name[4]);   //for wheel straight
            break;
        case THREE_OF_KIND:
            printf ("You have three %s with %s kicker\n",
                    plural_rank_name[rank->three_kind],
                    single_rank_name[rank->high_card[0]]);
            break;
        case TWO_PAIRS:
            printf ("You have two pairs %s and %s with %s kicker\n",
                    plural_rank_name[rank->high_pair],
                    plural_rank_name[rank->low_pair],
                    single_rank_name[rank->high_card[0]]);
            break;
        case PAIR:
            printf ("You have pair of %s with %s kicker\n",
                    plural_rank_name[rank->high_pair],
                    single_rank_name[rank->high_card[0]]);
            break;
        case HIGH_CARD:
            printf ("You have high card %s\n",
                    single_rank_name[rank->high_card[0]]);
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

static void print_board (board_t *board)
{
    print_card (board->card[0]);
    print_card (board->card[1]);
    print_card (board->card[2]);
    print_card (board->card[3]);
    print_card (board->card[4]);
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

static void get_board (board_t *board)
{
    char a[3];
    int i;
    for (i = 0; i < 5; i++) {
        scanf ("%s", a);
        board->card[i].rank = char_to_rank (a[0]);
        board->card[i].suit = char_to_suit (a[1]);
        board->card[i].bit = card_bit[board->card[i].rank];
    }
}

static void board_to_value (board_t *board, hand_value_t *value)
{
    int i;
    memset (value, 0, sizeof (*value));
    for (i = 0; i < 5; i++) {
        value->straight |= board->card[i].bit;
        value->flush[board->card[i].suit]++;
        value->ranks[board->card[i].rank]++;
    }
}

static void hand_to_value (hand_t *hand, board_t *board, hand_value_t *value)
{
    int i;
    board_to_value (board, value);
    for (i = 0; i < 2; i++) {
        value->straight |= hand->card[i].bit;
        value->flush[hand->card[i].suit]++;
        value->ranks[hand->card[i].rank]++;
    }
}

/***
 * returns:
 *  0 - no flush
 *  1 - flush
***/
static u8 is_flush (hand_value_t *value)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (value->flush[i] >= 5)
            return 1;
    }
    return 0;
}

/***
 * returns:
 *  0 - no straight
 *  1 - regular straight
 *  2 - wheel (Ace to Five)
***/
static u8 is_straight (hand_value_t *value)
{
    int i;
    for (i = 0; i < 9; i++) {
        if ((value->straight & straight_template[i]) == straight_template[i])
            return 1;
    }
    return ((value->straight & straight_template[9]) == straight_template[9]) ? 2 : 0;
}

static void comprehend_rank (hand_rank_t *rank)
{
    if (rank->straight && rank->flush) {
        if (rank->straight == 1 && rank->high_card[0] == 13)
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

static void value_to_rank (hand_value_t *value, hand_rank_t *rank)
{
    int i, j = 0;
    memset (rank, 0, sizeof (*rank));
    rank->flush = is_flush (value);
    rank->straight = is_straight (value);
    for (i = 13; i > 0; i--) {
        switch (value->ranks[i]) {
            case 4:
                if (!rank->four_kind)
                    rank->four_kind = i;
                break;
            case 3:
                if (!rank->three_kind)
                    rank->three_kind = i;
                else if (!rank->high_pair)
                    rank->high_pair = i;
                break;
            case 2:
                if (!rank->high_pair) 
                    rank->high_pair = i;
                else if (!rank->low_pair) 
                    rank->low_pair = i;
                else if (j < 5)
                    rank->high_card[j++] = i;
                break;
            case 1:
                if (j < 5)
                    rank->high_card[j++] = i;
                break;
            default:
                break;
        }
    }
    comprehend_rank (rank);
}

showdown_t compare_card_rank (u8 hero, u8 villain)
{
    return (hero > villain) ? win :
           (hero < villain) ? lose : tie;
}

showdown_t compare_card_ranks (u8 *hero, u8 *villain, int limit)
{
    int i;
    for (i = 0; i < limit; i++) {
        if (*hero > *villain)
            return win;
        else if (*hero < *villain)
            return lose;
        hero++;
        villain++;
    }
    return tie;
}

showdown_t compare_same_ranks (hand_rank_t *hero, hand_rank_t *villain)
{
    showdown_t tmp;
    switch (hero->rank) {
        case ROYAL_FLUSH:
            return tie;
        case STRAIGHT_FLUSH:
            return compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case FOUR_OF_KIND:
            tmp = compare_card_rank (hero->four_kind, villain->four_kind);
            return (tmp != tie) ? tmp :
                compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case FULL_HOUSE:
            tmp = compare_card_rank (hero->three_kind, villain->three_kind);
            return (tmp != tie) ? tmp :
                compare_card_rank (hero->high_pair, villain->high_pair);
        case FLUSH:
            return compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case STRAIGHT:
            return compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case THREE_OF_KIND:
            tmp = compare_card_rank (hero->three_kind, villain->three_kind);
            if (tmp != tie)
                return tmp;
            tmp = compare_card_rank (hero->high_card[0], villain->high_card[0]);
            return (tmp != tie) ? tmp :
                compare_card_rank (hero->high_card[1], villain->high_card[1]);
        case TWO_PAIRS:
            tmp = compare_card_rank (hero->high_pair, villain->high_pair);
            if (tmp != tie)
                return tmp;
            tmp = compare_card_rank (hero->low_pair, villain->low_pair);
            return (tmp != tie) ? tmp :
                compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case PAIR:
            tmp = compare_card_rank (hero->high_pair, villain->high_pair);
            return (tmp != tie) ? tmp :
                compare_card_ranks (hero->high_card, villain->high_card, 3);
        case HIGH_CARD:
            return compare_card_ranks (hero->high_card, villain->high_card, 5);
        default:
            break;
    }
    return tie;
}

showdown_t compare_ranks (hand_rank_t *hero, hand_rank_t *villain)
{
    if (hero->rank > villain->rank)
        return win;
    else if (hero->rank < villain->rank)
        return lose;
    else
        return compare_same_ranks (hero, villain);
}

int main (void)
{
    hand_t hand;
    board_t board;
    hand_value_t value;
    hand_rank_t hero;
    hand_rank_t villain;
    showdown_t result;

    printf ("enter hand: ");
    get_hand (&hand);
    printf ("enter board: ");
    get_board (&board);
    print_hand (&hand);
    print_board (&board);
    hand_to_value (&hand, &board, &value);
    value_to_rank (&value, &hero);
    board_to_value (&board, &value);
    value_to_rank (&value, &villain);
    printf ("You have: \n");
    print_rank (&hero);
    printf ("Board reads: \n");
    print_rank (&villain);
    result = compare_ranks (&hero, &villain);
    if (result == win)
        printf ("You won\n");
    else
        printf ("You can't beat the board\n");
    return 0;
}
