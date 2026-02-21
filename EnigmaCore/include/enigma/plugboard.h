#ifndef ENIGMACORE_PLUGBOARD_H
#define ENIGMACORE_PLUGBOARD_H

#include "helpers.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t wiring[ALPHABET_SIZE];
} Plugboard;

void PlugboardInit(Plugboard* plugboard);
void PlugboardAddCable(Plugboard* plugboard, char a, char b);
int PlugboardForward(const Plugboard* plugboard, int input_index);

#ifdef __cplusplus
}
#endif

#endif //ENIGMACORE_PLUGBOARD_H