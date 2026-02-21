#include "enigma_core_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "enigma/enigma.h"
#include "enigma/constants.h"
#include <string.h>
#include <stdio.h>

static EnigmaMachine enigma;

extern UART_HandleTypeDef huart2;

static void UART_SendString(const char* str)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

void EnigmaSetup(void)
{
  EnigmaInit(&enigma);

  Plugboard plugboard;
  PlugboardInit(&plugboard);
  EnigmaSetPlugboard(&enigma, &plugboard);

  Reflector reflector;
  ReflectorInit(&reflector, kEnigmaReflector_B_Wiring);
  EnigmaSetReflector(&enigma, &reflector);

  Rotor temp_rotor;

  RotorInit(&temp_rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);
  RotorSetPosition(&temp_rotor, 'A');
  EnigmaAddRotor(&enigma, &temp_rotor);

  RotorInit(&temp_rotor, kEnigmaRotor_II_Wiring, kEnigmaRotor_II_Notch, 0);
  RotorSetPosition(&temp_rotor, 'A');
  EnigmaAddRotor(&enigma, &temp_rotor);

  RotorInit(&temp_rotor, kEnigmaRotor_III_Wiring, kEnigmaRotor_III_Notch, 0);
  RotorSetPosition(&temp_rotor, 'A');
  EnigmaAddRotor(&enigma, &temp_rotor);
}

void EnigmaTask(void *arg)
{
  char text_buffer[64] = "RTOS RUNNING";
  char output_buffer[128];

  char encrypted[64];
  strcpy(encrypted, text_buffer);

  EnigmaEncryptString(&enigma, encrypted);

  snprintf(output_buffer, sizeof(output_buffer), "%s -> %s", text_buffer, encrypted);

  UART_SendString(output_buffer);

  osDelay(1000);
}