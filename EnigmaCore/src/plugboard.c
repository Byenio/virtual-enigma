#include "enigma/plugboard.h"
#include <ctype.h>

void PlugboardInit(Plugboard* plugboard)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        plugboard->wiring[i] = i;
    }
}

void PlugboardAddCable(Plugboard* plugboard, char a, char b)
{
    int idx_a = a - 'A';
    int idx_b = b - 'A';

    if (idx_a < 0 || idx_a >= ALPHABET_SIZE || idx_b < 0 || idx_b >= ALPHABET_SIZE) return;

    plugboard->wiring[idx_a] = idx_b;
    plugboard->wiring[idx_b] = idx_a;
}

int PlugboardForward(const Plugboard* plugboard, int input_index)
{
    return plugboard->wiring[input_index];
}