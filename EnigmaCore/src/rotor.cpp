#include "enigma/rotor.h"
#include "helpers.h"
#include <cassert>
#include <vector>

namespace enigma
{
  Rotor::Rotor(const std::string_view wiring, const char notch, const int ring_setting)
    : position_(0)
    , ring_setting_(ring_setting)
  {
    assert(wiring.length() == 26
      && "Rotor wiring must be 26 characters");

    std::vector<bool> seen_outputs(26, false);

    notch_index_ = notch - 'A';

    for (int i = 0; i < 26; i++)
    {
      const int output_index = wiring[i] - 'A';

      assert(output_index >= 0
        && output_index < 26
        && "Wiring contains invalid characters");

      assert(!seen_outputs[output_index]
        && "Rotor wiring maps two inputs to the same output");

      seen_outputs[output_index] = true;
      forward_wiring_[i] = output_index;
      backward_wiring_[output_index] = i;
    }
  }

  void Rotor::SetPosition(const char position)
  {
    position_ = position - 'A';
  }

  char Rotor::GetPosition() const
  {
    return static_cast<char>(position_ + 'A');
  }

  void Rotor::Step()
  {
    position_ = (position_ + 1) % 26;
  }

  bool Rotor::IsOnNotch() const
  {
    return position_ == notch_index_;
  }

  int Rotor::Forward(const int input_index) const
  {
    const int shift = position_ - ring_setting_;
    const int contact_index = helpers::Mod26(input_index + shift);
    const int output_pin = forward_wiring_[contact_index];

    return helpers::Mod26(output_pin - shift);
  }

  int Rotor::Backward(const int input_index) const
  {
    const int shift = position_ - ring_setting_;
    const int contact_index = helpers::Mod26(input_index + shift);
    const int output_pin = backward_wiring_[contact_index];

    return helpers::Mod26(output_pin - shift);
  }
}
