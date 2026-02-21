#include <gtest/gtest.h>
#include "enigma/enigma.h"
#include "enigma/constants.h"
#include <string>

class EnigmaMachineTest : public ::testing::Test
{
protected:
    EnigmaMachine machine{};

    void SetUp() override
    {
        EnigmaInit(&machine);

        Reflector r;
        ReflectorInit(&r, kEnigmaReflector_B_Wiring);
        EnigmaSetReflector(&machine, &r);

        Plugboard p;
        PlugboardInit(&p);
        EnigmaSetPlugboard(&machine, &p);

        // Add Rotors I, II, III
        Rotor temp;

        RotorInit(&temp, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);
        RotorSetPosition(&temp, 'A');
        EnigmaAddRotor(&machine, &temp);

        RotorInit(&temp, kEnigmaRotor_II_Wiring, kEnigmaRotor_II_Notch, 0);
        RotorSetPosition(&temp, 'A');
        EnigmaAddRotor(&machine, &temp);

        RotorInit(&temp, kEnigmaRotor_III_Wiring, kEnigmaRotor_III_Notch, 0);
        RotorSetPosition(&temp, 'A');
        EnigmaAddRotor(&machine, &temp);
    }
};

TEST_F(EnigmaMachineTest, EncryptsAAAAAtoBDZGO)
{
    char buffer[] = "AAAAA";
    EnigmaEncryptString(&machine, buffer);
    EXPECT_STREQ(buffer, "BDZGO");
}

TEST_F(EnigmaMachineTest, EncryptionIsSymmetric)
{
    std::string original = "HELLO";
    char buffer[10];
    strcpy(buffer, original.c_str());

    // Encrypt
    EnigmaEncryptString(&machine, buffer);
    std::string cyphertext = buffer;

    // Create Receiver (New Machine)
    EnigmaMachine receiver;
    EnigmaInit(&receiver);

    Reflector r;
    ReflectorInit(&r, kEnigmaReflector_B_Wiring);
    EnigmaSetReflector(&receiver, &r);

    Plugboard p;
    PlugboardInit(&p);
    EnigmaSetPlugboard(&receiver, &p);

    Rotor temp;
    RotorInit(&temp, kEnigmaRotor_I_Wiring, kEnigmaRotor_I_Notch, 0);
    RotorSetPosition(&temp, 'A');
    EnigmaAddRotor(&receiver, &temp);

    RotorInit(&temp, kEnigmaRotor_II_Wiring, kEnigmaRotor_II_Notch, 0);
    RotorSetPosition(&temp, 'A');
    EnigmaAddRotor(&receiver, &temp);

    RotorInit(&temp, kEnigmaRotor_III_Wiring, kEnigmaRotor_III_Notch, 0);
    RotorSetPosition(&temp, 'A');
    EnigmaAddRotor(&receiver, &temp);

    // Decrypt
    char decrypt_buffer[10];
    strcpy(decrypt_buffer, cyphertext.c_str());
    EnigmaEncryptString(&receiver, decrypt_buffer);

    EXPECT_STREQ(decrypt_buffer, original.c_str());
}