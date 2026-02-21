#include "enigma/reflector.h"
#include <ctype.h>

void ReflectorInit(Reflector* reflector, const char* wiring_str)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        reflector->wiring[i] = wiring_str[i] - 'A';
    }
}

int ReflectorReflect(const Reflector* reflector, int input_index)
{
    return reflector->wiring[input_index];
}