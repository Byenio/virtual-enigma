#include "enigma_core_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "enigma/enigma.h"
#include "enigma/constants.h"
#include <string.h>
#include <stdio.h>

static EnigmaMachine enigma;

static char current_reflector_setup = 'B';

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

#define NEXTION_RX_BUFFER_SIZE 128
#define BUFFER_SIZE 64
#define NEXTION_COLOR_WHITE 65535
#define NEXTION_COLOR_GRAY 50712
#define NEXTION_COLOR_GREEN 2016

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

  osDelay(10);
}

void Nextion_SetText(char* object_name, char* text)
{
  char cmd[100];
  snprintf(cmd, sizeof(cmd), "%s.txt=\"%s\"", object_name, text);
  Nextion_SendCommand(cmd);
}

void Nextion_SetColor(char* object_name, const uint16_t color)
{
  char cmd[100];
  snprintf(cmd, sizeof(cmd), "%s.bco=%u", object_name, color);
  Nextion_SendCommand(cmd);
}

void Nextion_SetStyleSelected(char* object_name)
{
  Nextion_SetColor(object_name, NEXTION_COLOR_GREEN);
}

void Nextion_SetStyleDefault(char* object_name)
{
  Nextion_SetColor(object_name, NEXTION_COLOR_GRAY);
}

void HAL_UART_RxCpltCallback(const UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    if (nextion_rx_byte == '#' || nextion_idx >= NEXTION_RX_BUFFER_SIZE - 1)
    {
      nextion_buffer[nextion_idx] = '\0';

      if (inputQueueHandle != NULL && nextion_idx > 3)
      {
        osMessageQueuePut(inputQueueHandle, nextion_buffer, 0, 0);
      }
      nextion_idx = 0;
    }
    else if (nextion_rx_byte >= 32 && nextion_rx_byte <= 126)
    {
      nextion_buffer[nextion_idx++] = nextion_rx_byte;
    }

    HAL_UART_Receive_IT(&huart1, &nextion_rx_byte, 1);
  }
}

static void UpdateRotorDisplays()
{
  char temp_position[2];
  char obj_name[16];

  for (uint8_t i = 0; i < 3; i++)
  {
    temp_position[0] = RotorGetPosition(&enigma.rotors[i]);
    temp_position[1] = '\0';

    snprintf(obj_name, sizeof(obj_name), "r%d_current", i);
    Nextion_SetText(obj_name, temp_position);
  }
}

static void UpdateReflectorDisplay(void)
{
  Nextion_SetStyleDefault("refA");
  Nextion_SetStyleDefault("refB");
  Nextion_SetStyleDefault("refC");

  char obj_name[5];
  snprintf(obj_name, sizeof(obj_name), "ref%c", current_reflector_setup);
  Nextion_SetStyleSelected(obj_name);
}

void EnigmaResetLogic(void)
{
  EnigmaInit(&enigma);

  Plugboard plugboard;
  PlugboardInit(&plugboard);
  EnigmaSetPlugboard(&enigma, &plugboard);

  Reflector reflector;
  ReflectorInit(&reflector, kEnigmaReflector_B_Wiring);
  EnigmaSetReflector(&enigma, &reflector);

  current_reflector_setup = 'B';

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

void EnigmaSetup(void)
{
  EnigmaResetLogic();

  if (inputQueueHandle == NULL) {
    inputQueueHandle = osMessageQueueNew(5, 64, NULL);
  }

  HAL_UART_Receive_IT(&huart1, &nextion_rx_byte, 1);
}

static void HandleMessage(const char* payload);
static void HandleConfiguration(const char* payload);
static void HandleRotorConfiguration(const char* payload);
static void HandleReflectorConfiguration(const char* payload);
static void HandleCommand(const char* payload);
static void ResetSystem(void);

void EnigmaTask(void *arg)
{
  char input_buffer[BUFFER_SIZE];

  osDelay(1000);

  UpdateRotorDisplays();

  for (;;)
  {
    if (osMessageQueueGet(inputQueueHandle, input_buffer, NULL, osWaitForever) == osOK)
    {
      if (strncmp(input_buffer, "MSG:", 4) == 0)
      {
        HandleMessage(input_buffer + 4);
      }
      else if (strncmp(input_buffer, "CFG:", 4) == 0)
      {
        HandleConfiguration(input_buffer + 4);
      }
      else if (strncmp(input_buffer, "CMD:", 4) == 0)
      {
        HandleCommand(input_buffer + 4);
      }
    }
  }
}

static void HandleMessage(const char* payload)
{
  char output_buffer[BUFFER_SIZE];

  strcpy(output_buffer, payload);
  EnigmaEncryptString(&enigma, output_buffer);

  Nextion_SetText("output_box", output_buffer);

  UpdateRotorDisplays();
}

static void HandleConfiguration(const char* payload)
{
  if (strncmp(payload, "ROT:", 4) == 0)
  {
    HandleRotorConfiguration(payload + 4);
  }
  else if (strncmp(payload, "REF:", 4) == 0)
  {
    HandleReflectorConfiguration(payload + 4);
  }
}

static void HandleRotorConfiguration(const char* payload)
{
  char rotor;
  char action[5];

  if (sscanf(payload, "%c:%4s", &rotor, action) == 2)
  {
    const uint8_t rotor_idx = rotor - '0';

    if (rotor_idx < 0 || rotor_idx > 2)
    {
      return;
    }

    if (strcmp(action, "UP") == 0)
    {
      RotorStep(&enigma.rotors[rotor_idx], 1);
    }
    else if (strcmp(action, "DOWN") == 0)
    {
      RotorStep(&enigma.rotors[rotor_idx], -1);
    }

    UpdateRotorDisplays();
  }
}

static void HandleReflectorConfiguration(const char* payload)
{
  if (strcmp(payload, "A") == 0)
  {
    Reflector reflector;
    ReflectorInit(&reflector, kEnigmaReflector_A_Wiring);
    EnigmaSetReflector(&enigma, &reflector);
    current_reflector_setup = 'A';
  }
  else if (strcmp(payload, "B") == 0)
  {
    Reflector reflector;
    ReflectorInit(&reflector, kEnigmaReflector_B_Wiring);
    EnigmaSetReflector(&enigma, &reflector);
    current_reflector_setup = 'B';
  }
  else if (strcmp(payload, "C") == 0)
  {
    Reflector reflector;
    ReflectorInit(&reflector, kEnigmaReflector_C_Wiring);
    EnigmaSetReflector(&enigma, &reflector);
    current_reflector_setup = 'C';
  }

  UpdateReflectorDisplay();
}

static void HandleCommand(const char* payload)
{
  if (strcmp(payload, "RESET") == 0)
  {
    ResetSystem();
  }
  else if (strcmp(payload, "UPDATE") == 0)
  {
    UpdateRotorDisplays();
    UpdateReflectorDisplay();
  }
}

static void ResetSystem(void)
{
  EnigmaResetLogic();

  Nextion_SetText("input_box", "INPUT");
  Nextion_SetText("output_box", "OUTPUT");

  UpdateRotorDisplays();
  UpdateReflectorDisplay();
}