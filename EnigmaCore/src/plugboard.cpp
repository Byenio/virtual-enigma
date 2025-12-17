#include "enigma/plugboard.h"

#include <cassert>
#include <numeric>

namespace enigma
{
  Plugboard::Plugboard()
  {
    Reset();
  }

  void Plugboard::AddCable(const char a, const char b)
  {
    const int input_index = a - 'A';
    const int output_index = b - 'A';

    assert(input_index >= 0
      && input_index < 26
      && "Invalid input character");

    assert(output_index >= 0
      && output_index < 26
      && "Invalid output character");

    assert(input_index != output_index
      && "Input and output characters are the same");

    const bool is_input_free = (wiring_[input_index] == input_index);
    const bool is_output_free = (wiring_[output_index] == output_index);

    assert(is_input_free
      && "Input character is already connected");

    assert(is_output_free
      && "Output character is already connected");

    wiring_[input_index] = output_index;
    wiring_[output_index] = input_index;
  }

  void Plugboard::Reset()
  {
    std::iota(wiring_.begin(), wiring_.end(), 0);
  }

  int Plugboard::Forward(const int input_index) const
  {
    return wiring_[input_index];
  }
}
