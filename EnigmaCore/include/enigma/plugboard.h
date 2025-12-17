#ifndef ENIGMACORE_PLUGBOARD_H
#define ENIGMACORE_PLUGBOARD_H

#include <array>
#include <vector>
#include <utility>

namespace enigma
{
  class Plugboard
  {
  public:
    Plugboard();

    void AddCable(char a, char b);
    void Reset();

    int Forward(int input_index) const;

  private:
    std::array<int, 26> wiring_ {};
  };
}

#endif //ENIGMACORE_PLUGBOARD_H