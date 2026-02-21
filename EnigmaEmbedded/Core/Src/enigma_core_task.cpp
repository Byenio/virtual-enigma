#include "enigma_core_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "enigma/constants.h"
#include <string>
#include <vector>

enigma::EnigmaMachine machine;

extern UART_HandleTypeDef huart2;

void SendString(const std::string& str)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)str.c_str(), str.length(), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

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