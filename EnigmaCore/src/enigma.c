#include "enigma/enigma.h"
#include <ctype.h>
#include <stdbool.h>

void EnigmaInit(EnigmaMachine* enigma)
{
    enigma->rotor_count = 0;
}

void EnigmaSetReflector(EnigmaMachine* enigma, const Reflector* reflector)
{
    enigma->reflector = *reflector;
}

void EnigmaSetPlugboard(EnigmaMachine* enigma, const Plugboard* plugboard)
{
    enigma->plugboard = *plugboard;
}

void EnigmaAddRotor(EnigmaMachine* enigma, const Rotor* rotor)
{
    if (enigma->rotor_count < MAX_ROTORS)
    {
        enigma->rotors[enigma->rotor_count] = *rotor;
        enigma->rotor_count++;
    }
}

static void StepRotors(EnigmaMachine* enigma)
{
    int count = enigma->rotor_count;
    if (count < 3) return;

    bool should_step[MAX_ROTORS] = {false};

    // The fast rotor (rightmost) always steps
    should_step[count - 1] = true;

    for (int i = count - 1; i > 0; --i) {
        if (RotorIsOnNotch(&enigma->rotors[i])) {
            should_step[i - 1] = true;

            // Double step behavior
            if (i != count - 1) {
                should_step[i] = true;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (should_step[i]) {
            RotorStep(&enigma->rotors[i]);
        }
    }
}

char EnigmaEncryptChar(EnigmaMachine* enigma, char c) {
    StepRotors(enigma);

    c = toupper(c);
    if (!isalpha(c)) return c;

    int index = c - 'A';

    // 1. Plugboard
    index = PlugboardForward(&enigma->plugboard, index);

    // 2. Rotors Right-to-Left (Forward)
    // iterating backwards because last added rotor is the rightmost
    for (int i = enigma->rotor_count - 1; i >= 0; i--) {
        index = RotorForward(&enigma->rotors[i], index);
    }

    // 3. Reflector
    index = ReflectorReflect(&enigma->reflector, index);

    // 4. Rotors Left-to-Right (Backward)
    for (int i = 0; i < enigma->rotor_count; i++) {
        index = RotorBackward(&enigma->rotors[i], index);
    }

    // 5. Plugboard
    index = PlugboardForward(&enigma->plugboard, index);

    return index + 'A';
}

void EnigmaEncryptString(EnigmaMachine* enigma, char* buffer)
{
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        buffer[i] = EnigmaEncryptChar(enigma, buffer[i]);
    }
}