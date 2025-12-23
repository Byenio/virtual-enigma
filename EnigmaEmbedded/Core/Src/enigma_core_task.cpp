#include "enigma_core_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "enigma/enigma.h"
#include <string>
#include <vector>

enigma::EnigmaMachine machine;

extern UART_HandleTypeDef huart2;

void SendString(const std::string& str)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)str.c_str(), str.length(), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

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

void enigma_setup()
{
  machine.SetReflector(enigma::Reflector(kReflector_B_Wiring));
  machine.SetPlugboard(enigma::Plugboard());
  machine.AddRotor(enigma::Rotor(kRotor_I_Wiring, kRotor_I_Notch));
  machine.AddRotor(enigma::Rotor(kRotor_II_Wiring, kRotor_II_Notch));
  machine.AddRotor(enigma::Rotor(kRotor_III_Wiring, kRotor_III_Notch));
}

void enigma_task(void *arg)
{

  const std::string clear_text = "RTOS running";

  std::string encrypted = machine.EncryptString(clear_text);
  SendString(clear_text + "->" + encrypted);
}