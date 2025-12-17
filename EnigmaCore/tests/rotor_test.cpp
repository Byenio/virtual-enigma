#include <gtest/gtest.h>
#include <string>
#include "../include/enigma/rotor.h"
#include "../include/enigma/constants.h"

namespace enigma
{
  TEST(RotorTest, InitializesToPositionA)
  {
    const Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch);
    EXPECT_EQ(rotor.GetPosition(), 'A');
  }

  TEST(RotorTest, ForwardEncryptionAtPositionA)
  {
    const Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch);

    constexpr int input_index = 0;
    constexpr int output_index = 4;
    EXPECT_EQ(rotor.Forward(input_index), output_index);
  }

  TEST(RotorTest, ForwardEncryptionAfterStepping) {
    Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch);

    rotor.Step();
    EXPECT_EQ(rotor.GetPosition(), 'B');

    constexpr int input_index = 0;
    constexpr int expected_output = 9;

    EXPECT_EQ(rotor.Forward(input_index), expected_output);
  }

  TEST(RotorTest, BackwardEncryptionInverse) {
    const Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch);

    constexpr int input_e = 4;
    constexpr int expected_a = 0;

    EXPECT_EQ(rotor.Backward(input_e), expected_a);
  }

  TEST(RotorTest, RingSettingAltersEncryption) {
    const Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch, 1);

    constexpr int input_a = 0;
    constexpr int expected_k = 10;

    EXPECT_EQ(rotor.Forward(input_a), expected_k);
  }

  TEST(RotorTest, NotchDetection) {
    Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch);

    rotor.SetPosition('P');
    EXPECT_FALSE(rotor.IsOnNotch());

    rotor.SetPosition('Q');
    EXPECT_TRUE(rotor.IsOnNotch());

    rotor.Step();
    EXPECT_FALSE(rotor.IsOnNotch());
  }

  TEST(RotorTest, PositionWrapsAround) {
    Rotor rotor(kRotor_I_Wiring, kRotor_I_Notch);

    rotor.SetPosition('Z');
    rotor.Step();

    EXPECT_EQ(rotor.GetPosition(), 'A');
  }
}
