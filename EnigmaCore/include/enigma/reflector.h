#ifndef ENIGMACORE_REFLECTOR_H
#define ENIGMACORE_REFLECTOR_H

#include <array>
#include <string_view>

namespace enigma
{
  class Reflector
  {
  public:
    Reflector() = default;

    explicit Reflector(std::string_view wiring);

    int Reflect(int input_index) const;

  private:
    std::array<int, 26> wiring_ {};
  };
}

#endif //ENIGMACORE_REFLECTOR_H