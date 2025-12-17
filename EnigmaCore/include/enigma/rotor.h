#ifndef ENIGMACORE_ROTOR_H
#define ENIGMACORE_ROTOR_H

#include <array>
#include <string>
#include <string_view>

namespace enigma
{
  class Rotor
  {
  public:
    Rotor(std::string_view wiring, char notch, int ring_setting = 0);

    void SetPosition(char position);
    [[nodiscard]] char GetPosition() const;

    void Step();
    [[nodiscard]] bool IsOnNotch() const;

    [[nodiscard]] int Forward(int input_index) const;
    [[nodiscard]] int Backward(int input_index) const;

  private:
    std::array<int, 26> forward_wiring_ {};
    std::array<int, 26> backward_wiring_ {};

    int position_;
    int ring_setting_;
    int notch_index_;
  };
}

#endif //ENIGMACORE_ROTOR_H