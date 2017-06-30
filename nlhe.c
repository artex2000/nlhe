#include <stdio.h>
#include <string.h>

#include "nlhe.h"

int run_hand_vs_hand (void)
{
    hand_t hero;
    hand_t villain;
    rundown_t run;
    int cr;
    char c;

   // do {
        printf ("Hero: ");
        get_hand (&hero);
        printf ("Villain: ");
        get_hand (&villain);

        hand_vs_hand (&hero, &villain, &run);

        printf ("Hero: ");
        print_hand (&hero);
        printf ("Villain: ");
        print_hand (&villain);
        cr = run.win + run.lose + run.tie;
        printf ("Win %d (%d%%)\n", run.win, run.win * 100 / cr);
        printf ("Lose %d (%d%%)\n", run.lose, run.lose * 100 / cr);
        printf ("Tie %d (%d%%)\n", run.tie, run.tie * 100 / cr);

        printf ("\nEnter q to quit, any other key to continue: ");
        c = getchar ();
        putchar (c);
  //  } while (c != 'q');

    return 0;
}

int run_meta_vs_meta (void)
{
    hand_t hero;
    hand_t villain[16];
    int size;
    rundown_t run;
    int cr;
    deck_t deck;
    char c;
    meta_hand_t m;

//    do {
        printf ("Hero: ");
        m = get_meta_hand ();
        init_deck (&deck);
        get_any_hand (&deck, &hero, m);
        remove_hand (&deck, &hero);

        printf ("Villain: ");
        m = get_meta_hand ();
        size = get_all_hands (&deck, villain, m);
        release_deck (&deck);

        hand_vs_range (&hero, villain, size, &run);

        printf ("Hero: ");
        print_hand_as_meta (&hero);
        printf ("Villain: ");
        print_meta (m);
        cr = run.win + run.lose + run.tie;
        printf ("Win %d (%d%%)\n", run.win, run.win * 100 / cr);
        printf ("Lose %d (%d%%)\n", run.lose, run.lose * 100 / cr);
        printf ("Tie %d (%d%%)\n", run.tie, run.tie * 100 / cr);
        printf ("Total %d\n", cr);

        printf ("\nEnter q to quit, any other key to continue: ");
        c = getchar ();
        putchar (c);
 //   } while (c != 'q');

    return 0;
}

int main (void)
{
    //return run_meta_vs_meta ();
    //return run_hand_vs_hand ();
    test_specific();
    return 0;
}
