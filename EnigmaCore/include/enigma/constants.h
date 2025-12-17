#ifndef ENIGMACORE_CONSTANTS_H
#define ENIGMACORE_CONSTANTS_H

#include <array>
#include <string_view>

namespace enigma
{
  constexpr std::string_view kRotor_I_Wiring   = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
  constexpr std::string_view kRotor_II_Wiring  = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
  constexpr std::string_view kRotor_III_Wiring = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
  constexpr std::string_view kRotor_IV_Wiring  = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
  constexpr std::string_view kRotor_V_Wiring   = "VZBRGITYUPSDNHLXAWMJQOFECK";

  constexpr char kRotor_I_Notch    = 'Q';
  constexpr char kRotor_II_Notch   = 'E';
  constexpr char kRotor_III_Notch  = 'V';
  constexpr char kRotor_IV_Notch   = 'J';
  constexpr char kRotor_V_Notch    = 'Z';

  constexpr std::string_view kReflector_A_Wiring = "EJMZALYXVBWFCRQUONTSPIKHGD";
  constexpr std::string_view kReflector_B_Wiring = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
  constexpr std::string_view kReflector_C_Wiring = "FVPJIAOYEDRZXWGCTKUQSBNMHL";
}

#endif //ENIGMACORE_CONSTANTS_H