#ifndef ENIGMACORE_REFLECTOR_H
#define ENIGMACORE_REFLECTOR_H

#include "helpers.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t wiring[ALPHABET_SIZE];
} Reflector;

void ReflectorInit(Reflector* reflector, const char* wiring_str);
int ReflectorReflect(const Reflector* reflector, int input_index);

#ifdef __cplusplus
}
#endif

#endif //ENIGMACORE_REFLECTOR_H