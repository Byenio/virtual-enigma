#ifndef ENIGMACORE_ROTOR_H
#define ENIGMACORE_ROTOR_H

#include "helpers.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t forward_wiring[ALPHABET_SIZE];
    uint8_t backward_wiring[ALPHABET_SIZE];
    uint8_t notch_index;
    uint8_t position;
    uint8_t ring_setting;
} Rotor;

void RotorInit(Rotor* rotor, const char* wiring, char notch, int ring_setting);
void RotorSetPosition(Rotor* rotor, char position);
char RotorGetPosition(const Rotor* rotor);
void RotorStep(Rotor* rotor);
bool RotorIsOnNotch(const Rotor* rotor);
int RotorForward(const Rotor* rotor, int input_index);
int RotorBackward(const Rotor* rotor, int input_index);

#ifdef __cplusplus
}
#endif

#endif //ENIGMACORE_ROTOR_H