#include <stdio.h>
#include <string.h>

#include "nlhe.h"

int main (void)
{
    hand_t hero;
    hand_t villain;
    char c;
    
    do {
        printf ("enter hero hand: ");
        get_hand (&hero);
        printf ("enter villain hand: ");
        get_hand (&villain);

        hand_vs_hand (&hero, &villain);

        printf ("\nEnter q to quit, any other key to continue: ");
        c = getchar ();
        putchar (c);
    } while (c != 'q');

    return 0;
}
