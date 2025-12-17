#include "enigma/reflector.h"

#include <cassert>

namespace enigma
{
  Reflector::Reflector(const std::string_view wiring)
  {
    assert(wiring.length() == 26
      && "Reflector wiring must be 26 characters");

    for (int i = 0; i < 26; i++)
    {
      const int output_index = wiring[i] - 'A';
      wiring_[i] = output_index;
    }

    for (int i = 0; i < 26; i++)
    {
      const int output = wiring_[i];
      assert(wiring_[output] == i
        && "Reflector wiring is not reciprocal");
    }
  }

  int Reflector::Reflect(const int input_index) const
  {
    return wiring_[input_index];
  }
}
