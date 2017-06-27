#include <stdio.h>

#include "nlhe.h"

void test_random (void)
{
    deck_t deck;
    char c;

    hand_t hero;
    hand_t villain;
    board_t board;
    showdown_t result;

    rand_init ();

    do {
        init_deck (&deck);

        random_hand (&deck, &hero);
        remove_hand (&deck, &hero);
        random_hand (&deck, &villain);
        remove_hand (&deck, &villain);
        random_board (&deck, &board);
        remove_board (&deck, &board);

        print_hand (&hero);
        print_hand (&villain);
        print_board (&board);

        result = showdown (&hero, &villain, &board);

        if (result == WIN)
            printf ("Hero won\n");
        else if (result == LOSE)
            printf ("Hero lost\n");
        else
            printf ("Hero and Villain tie\n");

        printf ("\nEnter q to quit, any other key to continue: ");
        c = getchar ();
        putchar (c);
    } while (c != 'q');
}

void check (hand_t *hero, hand_t *villain, board_t *board, hand_rank_t *hr, hand_rank_t *vr, showdown_t rs)
{
    char c;

    print_hand (hero);
    print_hand (villain);
    print_board (board);

    printf ("Hero has: \n");
    print_rank (hr);
    printf ("Villain has: \n");
    print_rank (vr);

    printf ("result %s\n", (rs == WIN) ? "win" : (rs == LOSE) ? "lose" : "tie");
    c = getchar ();
}

