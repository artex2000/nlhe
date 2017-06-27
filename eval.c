#include <stdio.h>
#include <string.h>

#include "nlhe.h"


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
***/
static u8 is_straight (hand_value_t *value)
{
    int i, j = 13;
    for (i = 0; i < 10; i++, j--) {
        if ((value->straight & straight_template[i]) == straight_template[i])
            return j;
    }
    return 0;
}

static void comprehend_rank (hand_rank_t *rank)
{
    if (rank->straight && rank->flush) {
        if (rank->straight == ACE)
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

static showdown_t compare_card_rank (u8 hero, u8 villain)
{
    return (hero > villain) ? WIN :
           (hero < villain) ? LOSE : TIE;
}

static showdown_t compare_card_ranks (u8 *hero, u8 *villain, int limit)
{
    int i;
    for (i = 0; i < limit; i++) {
        if (*hero > *villain)
            return WIN;
        else if (*hero < *villain)
            return LOSE;
        hero++;
        villain++;
    }
    return TIE;
}

static showdown_t compare_same_ranks (hand_rank_t *hero, hand_rank_t *villain)
{
    showdown_t tmp;
    switch (hero->rank) {
        case ROYAL_FLUSH:
            return TIE;
        case STRAIGHT_FLUSH:
            return compare_card_rank (hero->straight, villain->straight);
        case FOUR_OF_KIND:
            tmp = compare_card_rank (hero->four_kind, villain->four_kind);
            return (tmp != TIE) ? tmp :
                compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case FULL_HOUSE:
            tmp = compare_card_rank (hero->three_kind, villain->three_kind);
            return (tmp != TIE) ? tmp :
                compare_card_rank (hero->high_pair, villain->high_pair);
        case FLUSH:
            return compare_card_ranks (hero->high_card, villain->high_card, 5);
        case STRAIGHT:
            return compare_card_rank (hero->straight, villain->straight);
        case THREE_OF_KIND:
            tmp = compare_card_rank (hero->three_kind, villain->three_kind);
            return (tmp != TIE) ? tmp :
                compare_card_ranks (hero->high_card, villain->high_card, 2);
        case TWO_PAIRS:
            tmp = compare_card_rank (hero->high_pair, villain->high_pair);
            if (tmp != TIE)
                return tmp;
            tmp = compare_card_rank (hero->low_pair, villain->low_pair);
            return (tmp != TIE) ? tmp :
                compare_card_rank (hero->high_card[0], villain->high_card[0]);
        case PAIR:
            tmp = compare_card_rank (hero->high_pair, villain->high_pair);
            return (tmp != TIE) ? tmp :
                compare_card_ranks (hero->high_card, villain->high_card, 3);
        case HIGH_CARD:
            return compare_card_ranks (hero->high_card, villain->high_card, 5);
        default:
            break;
    }
    return TIE;
}

static showdown_t compare_ranks (hand_rank_t *hero, hand_rank_t *villain)
{
    if (hero->rank > villain->rank)
        return WIN;
    else if (hero->rank < villain->rank)
        return LOSE;
    else
        return compare_same_ranks (hero, villain);
}

showdown_t showdown (hand_t *hero, hand_t *villain, board_t *board)
{
    hand_value_t value;
    hand_rank_t hero_r;
    hand_rank_t villain_r;

    hand_to_value (hero, board, &value);
    value_to_rank (&value, &hero_r);
    hand_to_value (villain, board, &value);
    value_to_rank (&value, &villain_r);

    return compare_ranks (&hero_r, &villain_r);
}
