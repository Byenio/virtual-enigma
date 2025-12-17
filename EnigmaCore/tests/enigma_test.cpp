#include <gtest/gtest.h>
#include "enigma/enigma.h"
#include "enigma/constants.h"

namespace enigma
{
    class EnigmaMachineTest : public ::testing::Test
    {
    protected:
        EnigmaMachine machine;

        void SetUp() override
        {
            machine.SetReflector(Reflector(kReflector_B_Wiring));

            machine.SetPlugboard(Plugboard());

            machine.AddRotor(Rotor(kRotor_I_Wiring, kRotor_I_Notch));
            machine.AddRotor(Rotor(kRotor_II_Wiring, kRotor_II_Notch));
            machine.AddRotor(Rotor(kRotor_III_Wiring, kRotor_III_Notch));
        }
    };

    TEST_F(EnigmaMachineTest, EncryptsAAAAAtoBDZGO)
    {
        const std::string input = "AAAAA";
        const std::string expected = "BDZGO";

        EXPECT_EQ(machine.EncryptString(input), expected);
    }

    TEST_F(EnigmaMachineTest, EncryptionIsSymmetric)
    {
        const std::string plaintext = "HELLO";
        const std::string cyphertext = machine.EncryptString(plaintext);

        EnigmaMachine receiver;
        receiver.SetReflector(Reflector(kReflector_B_Wiring));
        receiver.SetPlugboard(Plugboard());
        receiver.AddRotor(Rotor(kRotor_I_Wiring, kRotor_I_Notch));
        receiver.AddRotor(Rotor(kRotor_II_Wiring, kRotor_II_Notch));
        receiver.AddRotor(Rotor(kRotor_III_Wiring, kRotor_III_Notch));

        const std::string decrypted = receiver.EncryptString(cyphertext);

        EXPECT_EQ(decrypted, plaintext);
    }

    TEST_F(EnigmaMachineTest, PlugboardChangesOutput)
    {
        Plugboard pb;
        pb.AddCable('A', 'Z');
        machine.SetPlugboard(pb);

        const char output = machine.Encrypt('A');
        EXPECT_NE(output, 'B');
    }
}