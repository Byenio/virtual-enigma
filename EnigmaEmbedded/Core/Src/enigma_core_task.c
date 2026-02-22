#include "enigma_core_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "enigma/enigma.h"
#include "enigma/constants.h"
#include <string.h>
#include <stdio.h>

static EnigmaMachine enigma;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

#define NEXTION_RX_BUFFER_SIZE 128

uint8_t nextion_rx_byte;
char nextion_buffer[NEXTION_RX_BUFFER_SIZE];
volatile uint8_t nextion_idx = 0;
volatile uint8_t msg_received_flag = 0;

osMessageQueueId_t inputQueueHandle;

void Nextion_SendCommand(const char* cmd)
{
  HAL_UART_Transmit(&huart1, (uint8_t*)cmd, strlen(cmd), 100);
  const uint8_t terminator[] = { 0xFF, 0xFF, 0xFF };
  HAL_UART_Transmit(&huart1, terminator, 3, 100);
}

void Nextion_SetText(char* object_name, char* text)
{
  char cmd[100];
  snprintf(cmd, sizeof(cmd), "%s.txt=\"%s\"", object_name, text);
  Nextion_SendCommand(cmd);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    if (nextion_rx_byte == '#' || nextion_idx >= NEXTION_RX_BUFFER_SIZE - 1)
    {
      nextion_buffer[nextion_idx] = '\0';

      if (inputQueueHandle != NULL)
      {
        osMessageQueuePut(inputQueueHandle, nextion_buffer, 0, 0);
      }

      nextion_idx = 0;
    }
    else
    {
      nextion_buffer[nextion_idx++] = nextion_rx_byte;
    }

    HAL_UART_Receive_IT(&huart1, &nextion_rx_byte, 1);
  }
}

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

  inputQueueHandle = osMessageQueueNew(5, 64, NULL);
  HAL_UART_Receive_IT(&huart1, &nextion_rx_byte, 1);
}

void EnigmaTask(void *arg)
{
  char input_buffer[64];
  char output_buffer[64];
  char debug_msg[64];

  // char encrypted[64];
  // strcpy(encrypted, text_buffer);
  //
  // EnigmaEncryptString(&enigma, encrypted);
  //
  // snprintf(output_buffer, sizeof(output_buffer), "%s -> %s", text_buffer, encrypted);
  //
  // UART_SendString(output_buffer);
  //
  // osDelay(1000);

  for (;;)
  {
    osStatus_t status = osMessageQueueGet(inputQueueHandle, input_buffer, NULL, osWaitForever);

    if (status == osOK)
    {
      snprintf(debug_msg, sizeof(debug_msg), "[RX] %s", input_buffer);
      UART_SendString(debug_msg);

      strcpy(output_buffer, input_buffer);
      EnigmaEncryptString(&enigma, output_buffer);

      Nextion_SetText("output_box", output_buffer);

      snprintf(debug_msg, sizeof(debug_msg), "[OUT] %s", output_buffer);
      UART_SendString(debug_msg);
    }
  }
}