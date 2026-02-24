#include "enigma/rotor.h"
#include <ctype.h>

void RotorInit(Rotor* rotor, const char* wiring, char notch, int ring_setting)
{
    rotor->notch_index = notch - 'A';
    rotor->ring_setting = ring_setting;
    rotor->position = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        int output_index = wiring[i] - 'A';
        rotor->forward_wiring[i] = output_index;
        rotor->backward_wiring[output_index] = i;
    }
}

void RotorSetPosition(Rotor* rotor, char position)
{
    rotor->position = position - 'A';
}

char RotorGetPosition(const Rotor* rotor)
{
    return rotor->position + 'A';
}

void RotorStep(Rotor* rotor, const int8_t direction)
{
    if (direction == 1)
    {
        rotor->position = (rotor->position + 1) % ALPHABET_SIZE;
    }
    else if (direction == -1)
    {
        rotor->position = (rotor->position - 1 + ALPHABET_SIZE) % ALPHABET_SIZE;
    }
}

bool RotorIsOnNotch(const Rotor* rotor)
{
    return rotor->position == rotor->notch_index;
}

int RotorForward(const Rotor* rotor, int input_index)
{
    int shift = rotor->position - rotor-> ring_setting;
    int contact_index = ModAlphabetSize(input_index + shift);
    int output_pin = rotor->forward_wiring[contact_index];

    return ModAlphabetSize(output_pin - shift);
}

int RotorBackward(const Rotor* rotor, int input_index)
{
    int shift = rotor->position - rotor-> ring_setting;
    int contact_index = ModAlphabetSize(input_index + shift);
    int output_pin = rotor->backward_wiring[contact_index];

    return ModAlphabetSize(output_pin - shift);
}