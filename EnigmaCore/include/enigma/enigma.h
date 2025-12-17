#ifndef ENIGMACORE_ENIGMA_H
#define ENIGMACORE_ENIGMA_H

#include <vector>

#include "reflector.h"
#include "plugboard.h"
#include "rotor.h"

namespace enigma
{
  class EnigmaMachine
  {
  public:
    EnigmaMachine();

    void SetReflector(const Reflector& reflector);
    void SetPlugboard(const Plugboard& plugboard);
    void AddRotor(const Rotor& rotor);

    char Encrypt(char c);
    std::string EncryptString(std::string_view message);

  private:
    void StepRotors();

    std::vector<Rotor> rotors_;
    Reflector reflector_;
    Plugboard plugboard_;
  };
}

#endif //ENIGMACORE_ENIGMA_H