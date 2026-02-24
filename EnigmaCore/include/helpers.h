#ifndef ENIGMACORE_HELPERS_H
#define ENIGMACORE_HELPERS_H

#define ALPHABET_SIZE 26
#define MAX_ROTORS 4

static inline int ModAlphabetSize(const int x)
{
  return (x % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
}

#endif //ENIGMACORE_HELPERS_H