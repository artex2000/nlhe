#ifndef __NLHE_H__
#define __NLHE_H__

#define SPADES      0
#define HEARTS      1
#define CLUBS       2
#define DIAMONDS    3

#define DEUCE       1
#define THREE       2
#define FOUR        3
#define FIVE        4
#define SIX         5
#define SEVEN       6
#define EIGHT       7
#define NINE        8
#define TEN         9
#define JACK        10
#define QUEEN       11
#define KING        12
#define ACE         13

#define SUITED      0
#define OFF_SUIT    1

#define HIGH_CARD       0
#define PAIR            1
#define TWO_PAIRS       2
#define THREE_OF_KIND   3
#define STRAIGHT        4
#define FLUSH           5
#define FULL_HOUSE      6
#define FOUR_OF_KIND    7
#define STRAIGHT_FLUSH  8
#define ROYAL_FLUSH     9

#define SPADES_PRINT    '\u2660'
#define HEARTS_PRINT    '\u2665'
#define CLUBS_PRINT     '\u2663'
#define DIAMONDS_PRINT  '\u2666'

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef enum {
    LOSE,
    TIE,
    WIN
} showdown_t;

typedef struct {
    u32 bit   : 16;
    u32 rank  : 8;
    u32 suit  : 8;
} card_t;

typedef struct {
    card_t card[2];
} hand_t;

typedef struct {
    card_t card[3];
} flop_t;

typedef struct {
    card_t card[5];
} board_t;

typedef struct {
    u32 rank1    : 8;
    u32 rank2    : 8;
    u32 suited   : 8;
    u32 reserved : 8;
} hand_template_t;

typedef struct {
    u16 straight;
    u8  flush[4];
    u8  ranks[14]; //use ranks from 1 to 13 to have 0 reserved
} hand_value_t;

typedef struct {
    u64 low_pair   : 8;
    u64 high_pair  : 8;
    u64 three_kind : 8;
    u64 four_kind  : 8;
    u64 straight   : 8;
    u64 flush      : 8;
    u64 rank       : 8;
    u64 reserved   : 8;
    u8 high_card[5];
} hand_rank_t;

typedef struct {
    u8 *set;
    u8 size;
} deck_t;

typedef struct {
    int win;
    int lose;
    int tie;
} rundown_t;

void init_deck (deck_t *deck);
void remove_card (deck_t *deck, card_t card);
void remove_hand (deck_t *deck, hand_t *hand);
void remove_flop (deck_t *deck, flop_t *flop);
void remove_board (deck_t *deck, board_t *board);
int deck_to_cards (deck_t *deck, card_t **cards);
int is_in_deck (deck_t *deck, card_t card);

void rand_init (void);
card_t random_card (deck_t *deck);
void random_hand (deck_t *deck, hand_t *hand);
void random_board (deck_t *deck, board_t *board);

void get_board (board_t *board);
void get_hand (hand_t *hand);

void print_rank (hand_rank_t *rank);
void print_board (board_t *board);
void print_hand (hand_t *hand);

showdown_t showdown (hand_t *hero, hand_t *villain, board_t *board);
void hand_vs_hand (hand_t *hero, hand_t *villain);

extern const u16 card_bit[];


#endif
