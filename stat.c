#include <stdio.h>

#include "nlhe.h"

void hand_vs_hand (hand_t *hero, hand_t *villain)
{
    deck_t deck;
    card_t *remain;
    int cr;
    int win, lose, tie;
    int a, b, c, d, e;
    board_t board;
    showdown_t result;

    init_deck (&deck);
    remove_hand (&deck, hero);
    remove_hand (&deck, villain);
    cr =  deck_to_cards (&deck, &remain);

    win = lose = tie = 0;
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
                        if (result == WIN)
                            win++;
                        else if (result == LOSE)
                            lose++;
                        else
                            tie++;
                    }
    cr = win + lose + tie;
    printf ("Hero: ");
    print_hand (hero);
    printf ("Villain: ");
    print_hand (villain);
    printf ("Win %d (%d%%)\n", win, win * 100 / cr);
    printf ("Lose %d (%d%%)\n", lose, lose * 100 / cr);
    printf ("Tie %d (%d%%)\n", tie, tie * 100 / cr);
}


