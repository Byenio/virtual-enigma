#ifndef ENIGMACORE_PLUGBOARD_H
#define ENIGMACORE_PLUGBOARD_H

#include "helpers.h"
#include <stdint.h>

typedef struct
{
    uint8_t wiring[ALPHABET_SIZE];
} Plugboard;

void PlugboardInit(Plugboard* plugboard);
void PlugboardAddCable(Plugboard* plugboard, char a, char b);
int PlugboardForward(const Plugboard* plugboard, int input_index);

#endif //ENIGMACORE_PLUGBOARD_H