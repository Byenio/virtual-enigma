#ifndef ENIGMACORE_ENIGMA_H
#define ENIGMACORE_ENIGMA_H

#include "rotor.h"
#include "reflector.h"
#include "plugboard.h"

typedef struct
{
    Plugboard plugboard;
    Reflector reflector;
    Rotor rotors[MAX_ROTORS];
    int rotor_count;
} EnigmaMachine;

void EnigmaInit(EnigmaMachine* enigma);
void EnigmaSetReflector(EnigmaMachine* enigma, const Reflector* reflector);
void EnigmaSetPlugboard(EnigmaMachine* enigma, const Plugboard* plugboard);
void EnigmaAddRotor(EnigmaMachine* enigma, const Rotor* rotor);
char EnigmaEncryptChar(EnigmaMachine* enigma, char c);
void EnigmaEncryptString(EnigmaMachine* enigma, char* buffer);

#endif //ENIGMACORE_ENIGMA_H