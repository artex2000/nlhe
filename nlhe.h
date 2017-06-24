#ifndef __NLHE_H__
#define __NLHE_H__

#define SPADES      0
#define HEARTS      1
#define CLUBS       2
#define DIAMONDS    3

#define DEUCE       0
#define THREE       1
#define FOUR        2
#define FIVE        3
#define SIX         4
#define SEVEN       5
#define EIGHT       6
#define NINE        7
#define TEN         8
#define JACK        9
#define QUEEN       10
#define KING        11
#define ACE         12

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
    lose,
    tie,
    win
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

#endif
