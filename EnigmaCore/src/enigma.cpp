#include "enigma/enigma.h"

#include <cassert>
#include <ranges>

namespace enigma
{
  EnigmaMachine::EnigmaMachine() = default;

  void EnigmaMachine::SetReflector(const Reflector &reflector)
  {
    reflector_ = reflector;
  }

  void EnigmaMachine::SetPlugboard(const Plugboard &plugboard)
  {
    plugboard_ = plugboard;
  }

  /**
  * Adding a rotor pushes it at the end of rotors vector.
  * Last rotor is the rightmost and the fastest rotor (always rotates).
  */
  void EnigmaMachine::AddRotor(const Rotor &rotor)
  {
    rotors_.push_back(rotor);
  }

  char EnigmaMachine::Encrypt(char c)
  {
    StepRotors();

    c = static_cast<char>(std::toupper(c));

    int index = c - 'A';

    index = plugboard_.Forward(index);

    // forward signal - backwards through vector (right -> left)
    for (auto & rotor : std::ranges::reverse_view(rotors_))
    {
      index = rotor.Forward(index);
    }

    index = reflector_.Reflect(index);

    // backward signal - forwards through vector (left -> right)
    for (auto & rotor : rotors_)
    {
      index = rotor.Backward(index);
    }

    index = plugboard_.Forward(index);

    return static_cast<char>(index + 'A');
  }

  std::string EnigmaMachine::EncryptString(std::string_view message)
  {
    std::string result;
    result.reserve(message.length());

    for (char c : message)
    {
      c = static_cast<char>(std::toupper(c));
      if (c >= 'A' && c <= 'Z')
      {
        result += Encrypt(c);
      }
      else
      {
        result += c;
      }
    }

    return result;
  }

  void EnigmaMachine::StepRotors()
  {
    const size_t num_rotors = rotors_.size();

    assert(num_rotors >= 3
      && "Minimum number of rotors is 3");

    std::vector<bool> should_step(num_rotors, false);

    should_step[num_rotors - 1] = true;

    for (size_t i = num_rotors - 1; i > 0; --i)
    {
      if (Rotor& current = rotors_[i]; current.IsOnNotch())
      {
        should_step[i - 1] = true;

        // double step
        if (i != num_rotors - 1)
        {
          should_step[i] = true;
        }
      }
    }

    for (size_t i = 0; i < num_rotors; i++)
    {
      if (should_step[i])
      {
        rotors_[i].Step();
      }
    }
  }
}
