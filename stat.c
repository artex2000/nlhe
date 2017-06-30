#include <stdio.h>

#include "nlhe.h"

void hand_vs_hand (hand_t *hero, hand_t *villain, rundown_t *run)
{
    deck_t deck;
    card_t remain[52];
    int cr;
    int a, b, c, d, e;
    board_t board;
    showdown_t result;

    init_deck (&deck);
    remove_hand (&deck, hero);
    remove_hand (&deck, villain);
    cr =  deck_to_cards (&deck, remain);

    run->win = run->lose = run->tie = 0;
    for (a = 0; a < cr - 4; a++)
        for (b = a + 1; b < cr - 3; b++)
            for (c = b + 1; c < cr - 2; c++)
                for (d = c + 1; d < cr - 1; d++)
                    for (e = d + 1; e < cr; e++) {
                        board.card[0] = remain[a];
                        board.card[1] = remain[b];
                        board.card[2] = remain[c];
                        board.card[3] = remain[d];
                        board.card[4] = remain[e];

                        result = showdown (hero, villain, &board);
                        if (result == WIN) {
                            print_card (remain[a]);
                            print_card (remain[b]);
                            print_card (remain[c]);
                            print_card (remain[d]);
                            print_card (remain[e]);
                            printf ("- win\n");
                            run->win++;
                        } else if (result == LOSE) {
                            print_card (remain[a]);
                            print_card (remain[b]);
                            print_card (remain[c]);
                            print_card (remain[d]);
                            print_card (remain[e]);
                            printf ("- lose\n");
                            run->lose++;
                        } else {
                            print_card (remain[a]);
                            print_card (remain[b]);
                            print_card (remain[c]);
                            print_card (remain[d]);
                            print_card (remain[e]);
                            printf ("- tie\n");
                            run->tie++;
                        }
                    }
    release_deck (&deck);
    print_hand (hero);
    print_hand (villain);
    printf ("win %d, lose %d, tie %d\n", run->win, run->lose, run->tie);
}

void hand_vs_range (hand_t *hero, hand_t *villain, int size, rundown_t *run)
{
    rundown_t tmp;
    int i;

    run->win = run->lose = run->tie = 0;
    for (i = 0; i < size; i++) {
        hand_vs_hand (hero, &villain[i], &tmp);
        run->win += tmp.win;
        run->lose += tmp.lose;
        run->tie += tmp.tie;
    }
}

void get_any_hand (deck_t *deck, hand_t *hand, meta_hand_t meta)
{
    int i, j;
    card_t a, b;

    if (meta.suited) {
        for (i = 0; i < 4; i++) {
            a.rank = meta.rank1;
            b.rank = meta.rank2;
            a.suit = b.suit = i;
            if (is_in_deck (deck, a) && is_in_deck (deck, b)) {
                hand->card[0] = a;
                hand->card[1] = b;
                return;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            a.rank = meta.rank1;
            a.suit = i;
            if (is_in_deck (deck, a)) {
                for (j = 0; j < 4; j++) {
                    if (j == i)
                        continue;
                    b.rank = meta.rank2;
                    b.suit = j;
                    if (is_in_deck (deck, b)) {
                        hand->card[0] = a;
                        hand->card[1] = b;
                        return;
                    }
                }
            }
        }
    }
}

int get_all_hands (deck_t *deck, hand_t *hand, meta_hand_t meta)
{
    int i, j, c = 0;
    card_t a, b;

    if (meta.suited) {
        for (i = 0; i < 4; i++) {
            a.rank = meta.rank1;
            b.rank = meta.rank2;
            a.suit = b.suit = i;
            if (is_in_deck (deck, a) && is_in_deck (deck, b)) {
                hand[c].card[0] = a;
                hand[c++].card[1] = b;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            a.rank = meta.rank1;
            a.suit = i;
            if (is_in_deck (deck, a)) {
                for (j = 0; j < 4; j++) {
                    if (j == i)
                        continue;
                    b.rank = meta.rank2;
                    b.suit = j;
                    if (is_in_deck (deck, b)) {
                        hand[c].card[0] = a;
                        hand[c++].card[1] = b;
                    }
                }
            }
        }
    }
    return c;
}



