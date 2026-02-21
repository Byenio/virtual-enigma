#include <gtest/gtest.h>
#include "enigma/rotor.h"
#include "enigma/constants.h"

TEST(RotorTest, InitializesToPositionA)
{
  Rotor rotor;
  RotorInit(&rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);
  EXPECT_EQ(RotorGetPosition(&rotor), 'A');
}

TEST(RotorTest, ForwardEncryptionAtPositionA)
{
  Rotor rotor;
  RotorInit(&rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);

  const int input_index = 0;
  const int output_index = 4;
  EXPECT_EQ(RotorForward(&rotor, input_index), output_index);
}

TEST(RotorTest, ForwardEncryptionAfterStepping) {
  Rotor rotor;
  RotorInit(&rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);

  RotorStep(&rotor);
  EXPECT_EQ(RotorGetPosition(&rotor), 'B');

  const int input_index = 0;
  const int expected_output = 9;

  EXPECT_EQ(RotorForward(&rotor, input_index), expected_output);
}

TEST(RotorTest, BackwardEncryptionInverse) {
  Rotor rotor;
  RotorInit(&rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);

  const int input_e = 4;
  const int expected_a = 0;

  EXPECT_EQ(RotorBackward(&rotor, input_e), expected_a);
}

TEST(RotorTest, RingSettingAltersEncryption) {
  Rotor rotor;
  // Note: Ring setting is 1 ('B')
  RotorInit(&rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 1);

  const int input_a = 0;
  const int expected_k = 10;

  EXPECT_EQ(RotorForward(&rotor, input_a), expected_k);
}

TEST(RotorTest, NotchDetection) {
  Rotor rotor;
  RotorInit(&rotor, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);

  RotorSetPosition(&rotor, 'P');
  EXPECT_FALSE(RotorIsOnNotch(&rotor));

  // Rotor I notch is at Q
  RotorSetPosition(&rotor, 'Q');
  EXPECT_TRUE(RotorIsOnNotch(&rotor));

  RotorStep(&rotor);
  EXPECT_FALSE(RotorIsOnNotch(&rotor));
}